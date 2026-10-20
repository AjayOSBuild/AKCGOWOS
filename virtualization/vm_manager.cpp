#include "vm_manager.h"
#include "virtual_disk.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

VmManager::VmManager() {
    std::cout << "[VmManager] Initializing Virtual Machine Manager...\n";
    srand(time(nullptr));
}

std::string VmManager::GenerateRandomMac() const {
    std::stringstream ss;
    ss << "52:54:00"; 
    for (int i = 0; i < 3; ++i) ss << ":" << std::setfill('0') << std::setw(2) << std::hex << (rand() % 256);
    return ss.str();
}

bool VmManager::CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch) {
    if (vm_configs.find(name) != vm_configs.end()) return false;
    vm_configs[name] = {name, memory_mb, cpus, arch, {}, {}, 0, 0};
    return true;
}

bool VmManager::AttachDisk(const std::string& vm_name, const std::string& disk_path) {
    if (vm_configs.find(vm_name) == vm_configs.end()) return false;
    vm_configs[vm_name].disks.push_back(disk_path);
    return true;
}

bool VmManager::AttachNetwork(const std::string& vm_name, const std::string& type, const std::string& bridge) {
    if (vm_configs.find(vm_name) == vm_configs.end()) return false;
    vm_configs[vm_name].networks.push_back({type, GenerateRandomMac(), bridge});
    return true;
}

bool VmManager::SetResourceLimits(const std::string& vm_name, int cpu_quota_pct, int mem_limit_mb) {
    if (vm_configs.find(vm_name) == vm_configs.end()) return false;
    vm_configs[vm_name].cpu_quota_pct = cpu_quota_pct;
    vm_configs[vm_name].hard_mem_limit_mb = mem_limit_mb;
    return true;
}

bool VmManager::SnapshotVm(const std::string& vm_name, const std::string& snapshot_name) {
    auto config_it = vm_configs.find(vm_name);
    if (config_it == vm_configs.end()) return false;
    if (active_vms.find(vm_name) != active_vms.end() && active_vms[vm_name]->IsRunning()) {
        std::cerr << "[!] Cannot safely snapshot VM '" << vm_name << "'. VM must be powered off.\n";
        return false;
    }
    VirtualDiskManager disk_mgr;
    std::cout << "[VmManager] Initiating offline snapshot '" << snapshot_name << "' for VM '" << vm_name << "'...\n";
    bool success = true;
    for (const auto& disk : config_it->second.disks) {
        if (!disk_mgr.CreateSnapshot(disk, snapshot_name)) success = false;
    }
    if (success) std::cout << "[VmManager] VM snapshot successfully completed.\n";
    return success;
}

bool VmManager::RestoreVm(const std::string& vm_name, const std::string& snapshot_name) {
    auto config_it = vm_configs.find(vm_name);
    if (config_it == vm_configs.end()) {
        std::cerr << "[!] VM '" << vm_name << "' not found.\n";
        return false;
    }
    if (active_vms.find(vm_name) != active_vms.end() && active_vms[vm_name]->IsRunning()) {
        std::cerr << "[!] Cannot restore VM '" << vm_name << "' while it is running. Power off first.\n";
        return false;
    }
    VirtualDiskManager disk_mgr;
    std::cout << "[VmManager] Initiating restore to snapshot '" << snapshot_name << "' for VM '" << vm_name << "'...\n";
    bool success = true;
    for (const auto& disk : config_it->second.disks) {
        if (!disk_mgr.RestoreSnapshot(disk, snapshot_name)) success = false;
    }
    if (success) std::cout << "[VmManager] VM restore successfully completed.\n";
    return success;
}

bool VmManager::StartVm(const std::string& name) {
    auto config_it = vm_configs.find(name);
    if (config_it == vm_configs.end() || (active_vms.find(name) != active_vms.end() && active_vms[name]->IsRunning())) return false;
    const VmConfig& config = config_it->second;
    std::string binary = (config.architecture == "x86_64") ? "/usr/bin/qemu-system-x86_64" : "/usr/bin/qemu-system-aarch64";
    auto wrapper = std::make_shared<QemuWrapper>(binary);
    
    wrapper->AddArgument("-m"); wrapper->AddArgument(std::to_string(config.memory_mb));
    wrapper->AddArgument("-smp"); wrapper->AddArgument(std::to_string(config.cpus));
    wrapper->AddArgument("-name"); wrapper->AddArgument(config.name);
    wrapper->AddArgument("-display"); wrapper->AddArgument("none");

    if (wrapper->Start()) {
        active_vms[name] = wrapper;
        if (config.cpu_quota_pct > 0 || config.hard_mem_limit_mb > 0) {
            cgroup_mgr.ApplyLimits(name, wrapper->GetPid(), config.cpu_quota_pct, config.hard_mem_limit_mb);
        }
        return true;
    }
    return false;
}

bool VmManager::StopVm(const std::string& name) {
    auto active_it = active_vms.find(name);
    if (active_it != active_vms.end() && active_it->second->Stop()) {
        active_vms.erase(active_it);
        cgroup_mgr.RemoveLimits(name);
        return true;
    }
    return false;
}

void VmManager::ListVms() const {
    std::cout << "\n--- Configured Virtual Machines ---\n";
    for (const auto& pair : vm_configs) {
        std::cout << "- " << pair.second.name << " | Disks attached: " << pair.second.disks.size() << "\n";
    }
    std::cout << "-----------------------------------\n\n";
}

int main() {
    std::cout << "=== OS Project VM Restore Utility ===\n";
    VirtualDiskManager disk_mgr;
    VmManager vm_mgr;
    
    std::string test_disk = "/tmp/nexus_restore_test.qcow2";
    disk_mgr.CreateDisk(test_disk, "5G");

    vm_mgr.CreateVm("worker-node", 2048, 2);
    vm_mgr.AttachDisk("worker-node", test_disk);
    
    std::cout << "\n[Test] Creating a baseline snapshot...\n";
    vm_mgr.SnapshotVm("worker-node", "baseline-install");
    
    std::cout << "\n[Test] Attempting restore to baseline...\n";
    vm_mgr.RestoreVm("worker-node", "baseline-install");
    
    vm_mgr.StartVm("worker-node");
    
    std::cout << "\n[Test] Attempting restore while running (should be blocked for safety)...\n";
    vm_mgr.RestoreVm("worker-node", "baseline-install");
    
    vm_mgr.StopVm("worker-node");
    remove(test_disk.c_str());
    
    std::cout << "[+] VM Restore testing completed.\n";
    return 0;
}

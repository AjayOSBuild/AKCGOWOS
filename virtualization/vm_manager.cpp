#include "vm_manager.h"
#include "virtual_disk.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

VmManager::VmManager() {
    std::cout << "[VmManager] Initializing Virtual Machine Manager (Resource Limits Enabled)...\n";
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
    auto it = vm_configs.find(vm_name);
    if (it == vm_configs.end()) return false;
    it->second.disks.push_back(disk_path);
    return true;
}

bool VmManager::AttachNetwork(const std::string& vm_name, const std::string& type, const std::string& bridge) {
    auto it = vm_configs.find(vm_name);
    if (it == vm_configs.end()) return false;
    it->second.networks.push_back({type, GenerateRandomMac(), bridge});
    return true;
}

bool VmManager::SetResourceLimits(const std::string& vm_name, int cpu_quota_pct, int mem_limit_mb) {
    auto it = vm_configs.find(vm_name);
    if (it == vm_configs.end()) return false;
    it->second.cpu_quota_pct = cpu_quota_pct;
    it->second.hard_mem_limit_mb = mem_limit_mb;
    std::cout << "[VmManager] Configured limits for '" << vm_name << "': CPU " 
              << cpu_quota_pct << "%, RAM " << mem_limit_mb << " MB\n";
    return true;
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
        std::cout << "[VmManager] VM '" << name << "' started successfully.\n";
        
        // Apply cgroup limits to the newly spawned QEMU PID
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
        std::cout << "- " << pair.second.name << " | Limits -> CPU: " 
                  << (pair.second.cpu_quota_pct ? std::to_string(pair.second.cpu_quota_pct) + "%" : "Uncapped")
                  << ", RAM: " << (pair.second.hard_mem_limit_mb ? std::to_string(pair.second.hard_mem_limit_mb) + "MB" : "Uncapped") << "\n";
    }
    std::cout << "-----------------------------------\n\n";
}

int main() {
    std::cout << "=== OS Project VM Resource Limits Utility ===\n";
    VmManager vm_mgr;
    
    vm_mgr.CreateVm("restricted-node", 1024, 2);
    // Constrain to 50% CPU time and hard limit to 1200MB RAM to prevent OOM kills impacting the host
    vm_mgr.SetResourceLimits("restricted-node", 50, 1200); 
    
    vm_mgr.ListVms();
    vm_mgr.StartVm("restricted-node");
    
    sleep(1);
    vm_mgr.StopVm("restricted-node");
    
    std::cout << "[+] VM Resource Limits testing completed.\n";
    return 0;
}

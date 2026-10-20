#include "vm_manager.h"
#include "virtual_disk.h"
#include <iostream>
#include <unistd.h>

VmManager::VmManager() {
    std::cout << "[VmManager] Initializing Virtual Machine Manager...\n";
}

bool VmManager::CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch) {
    if (vm_configs.find(name) != vm_configs.end()) return false;
    vm_configs[name] = {name, memory_mb, cpus, arch, {}};
    return true;
}

bool VmManager::AttachDisk(const std::string& vm_name, const std::string& disk_path) {
    auto it = vm_configs.find(vm_name);
    if (it == vm_configs.end()) {
        std::cerr << "[!] VM '" << vm_name << "' not found.\n";
        return false;
    }
    it->second.disks.push_back(disk_path);
    std::cout << "[VmManager] Attached disk '" << disk_path << "' to VM '" << vm_name << "'.\n";
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

    // Mount attached disks
    for (const auto& disk : config.disks) {
        wrapper->AddArgument("-drive");
        wrapper->AddArgument("file=" + disk + ",format=qcow2,if=virtio");
    }

    if (wrapper->Start()) {
        active_vms[name] = wrapper;
        std::cout << "[VmManager] VM '" << name << "' started successfully with " << config.disks.size() << " disk(s).\n";
        return true;
    }
    return false;
}

bool VmManager::StopVm(const std::string& name) {
    auto active_it = active_vms.find(name);
    if (active_it != active_vms.end() && active_it->second->Stop()) {
        active_vms.erase(active_it);
        return true;
    }
    return false;
}

void VmManager::ListVms() const {
    std::cout << "\n--- Configured Virtual Machines ---\n";
    for (const auto& pair : vm_configs) {
        const VmConfig& c = pair.second;
        std::cout << "- " << c.name << " | Disks: " << c.disks.size() << "\n";
    }
    std::cout << "-----------------------------------\n\n";
}

int main() {
    std::cout << "=== OS Project VM Disk Subsystem Utility ===\n";
    VirtualDiskManager disk_mgr;
    VmManager vm_mgr;
    
    std::string test_disk = "/tmp/nexus_test_vm.qcow2";
    disk_mgr.CreateDisk(test_disk, "10G");

    vm_mgr.CreateVm("storage-node", 1024, 2);
    vm_mgr.AttachDisk("storage-node", test_disk);
    vm_mgr.ListVms();
    
    vm_mgr.StartVm("storage-node");
    
    std::cout << "[+] Allowing VM to boot and mount virtio block device...\n";
    sleep(2);
    
    vm_mgr.StopVm("storage-node");
    
    // Cleanup mock disk
    remove(test_disk.c_str());
    
    std::cout << "[+] VM Disk testing completed.\n";
    return 0;
}

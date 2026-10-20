#include "vm_manager.h"
#include "virtual_disk.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

VmManager::VmManager() {
    std::cout << "[VmManager] Initializing Virtual Machine Manager (Networking Enabled)...\n";
    srand(time(nullptr));
}

std::string VmManager::GenerateRandomMac() const {
    std::stringstream ss;
    ss << "52:54:00"; // Standard KVM/QEMU OUI
    for (int i = 0; i < 3; ++i) {
        ss << ":" << std::setfill('0') << std::setw(2) << std::hex << (rand() % 256);
    }
    return ss.str();
}

bool VmManager::CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch) {
    if (vm_configs.find(name) != vm_configs.end()) return false;
    vm_configs[name] = {name, memory_mb, cpus, arch, {}, {}};
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
    if (it == vm_configs.end()) {
        std::cerr << "[!] VM '" << vm_name << "' not found.\n";
        return false;
    }
    
    std::string mac = GenerateRandomMac();
    it->second.networks.push_back({type, mac, bridge});
    std::cout << "[VmManager] Attached '" << type << "' network interface (MAC: " << mac << ") to VM '" << vm_name << "'.\n";
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
    
    // Attach network interfaces
    int net_idx = 0;
    for (const auto& net : config.networks) {
        std::string net_id = "net" + std::to_string(net_idx++);
        
        if (net.type == "user") {
            wrapper->AddArgument("-netdev");
            wrapper->AddArgument("user,id=" + net_id);
        } else if (net.type == "tap") {
            wrapper->AddArgument("-netdev");
            wrapper->AddArgument("tap,id=" + net_id + ",br=" + net.bridge_name + ",helper=/usr/libexec/qemu-bridge-helper");
        }
        
        wrapper->AddArgument("-device");
        wrapper->AddArgument("virtio-net-pci,netdev=" + net_id + ",mac=" + net.mac_address);
    }

    if (wrapper->Start()) {
        active_vms[name] = wrapper;
        std::cout << "[VmManager] VM '" << name << "' started successfully with " 
                  << config.networks.size() << " network interface(s).\n";
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
        std::cout << "- " << c.name << " | Disks: " << c.disks.size() 
                  << " | Networks: " << c.networks.size() << "\n";
    }
    std::cout << "-----------------------------------\n\n";
}

int main() {
    std::cout << "=== OS Project VM Networking Subsystem Utility ===\n";
    VmManager vm_mgr;
    
    vm_mgr.CreateVm("net-node", 1024, 2);
    
    // Attach a standard NAT interface and a Bridged interface
    vm_mgr.AttachNetwork("net-node", "user");
    vm_mgr.AttachNetwork("net-node", "tap", "nexus-br0");
    
    vm_mgr.ListVms();
    
    vm_mgr.StartVm("net-node");
    sleep(1);
    vm_mgr.StopVm("net-node");
    
    std::cout << "[+] VM Networking testing completed.\n";
    return 0;
}

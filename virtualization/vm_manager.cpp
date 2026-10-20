#include "vm_manager.h"
#include <iostream>

VmManager::VmManager() {
    std::cout << "[VmManager] Initializing Virtual Machine Manager...\n";
}

bool VmManager::CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch) {
    if (vm_configs.find(name) != vm_configs.end()) {
        std::cerr << "[!] VM '" << name << "' already exists.\n";
        return false;
    }

    vm_configs[name] = {name, memory_mb, cpus, arch};
    std::cout << "[VmManager] Created VM configuration: " << name 
              << " (" << cpus << " CPUs, " << memory_mb << " MB RAM, " << arch << ")\n";
    return true;
}

bool VmManager::StartVm(const std::string& name) {
    auto config_it = vm_configs.find(name);
    if (config_it == vm_configs.end()) {
        std::cerr << "[!] Cannot start. VM '" << name << "' not found.\n";
        return false;
    }

    if (active_vms.find(name) != active_vms.end() && active_vms[name]->IsRunning()) {
        std::cerr << "[!] VM '" << name << "' is already running.\n";
        return false;
    }

    const VmConfig& config = config_it->second;
    std::string binary = (config.architecture == "x86_64") ? "/usr/bin/qemu-system-x86_64" : "/usr/bin/qemu-system-aarch64";
    
    auto wrapper = std::make_shared<QemuWrapper>(binary);
    
    // Construct QEMU arguments based on VM configuration
    wrapper->AddArgument("-m");
    wrapper->AddArgument(std::to_string(config.memory_mb));
    wrapper->AddArgument("-smp");
    wrapper->AddArgument(std::to_string(config.cpus));
    wrapper->AddArgument("-name");
    wrapper->AddArgument(config.name);
    wrapper->AddArgument("-display");
    wrapper->AddArgument("none"); // Headless for testing

    if (wrapper->Start()) {
        active_vms[name] = wrapper;
        std::cout << "[VmManager] VM '" << name << "' started successfully.\n";
        return true;
    }

    std::cerr << "[!] Failed to start VM '" << name << "'.\n";
    return false;
}

bool VmManager::StopVm(const std::string& name) {
    auto active_it = active_vms.find(name);
    if (active_it == active_vms.end() || !active_it->second->IsRunning()) {
        std::cerr << "[!] VM '" << name << "' is not currently running.\n";
        return false;
    }

    std::cout << "[VmManager] Requesting stop for VM '" << name << "'...\n";
    if (active_it->second->Stop()) {
        active_vms.erase(active_it);
        return true;
    }
    return false;
}

void VmManager::ListVms() const {
    std::cout << "\n--- Configured Virtual Machines ---\n";
    for (const auto& pair : vm_configs) {
        const VmConfig& config = pair.second;
        bool is_running = (active_vms.find(config.name) != active_vms.end() && active_vms.at(config.name)->IsRunning());
        
        std::cout << "- " << config.name << " | Arch: " << config.architecture 
                  << " | CPUs: " << config.cpus << " | RAM: " << config.memory_mb << " MB "
                  << "| Status: " << (is_running ? "RUNNING" : "STOPPED") << "\n";
    }
    std::cout << "-----------------------------------\n\n";
}

// Test main function
int main() {
    std::cout << "=== OS Project VM Manager Utility ===\n";
    VmManager manager;
    
    manager.CreateVm("nexus-test-node", 1024, 2);
    manager.CreateVm("legacy-win-app", 2048, 4);
    
    manager.ListVms();
    
    manager.StartVm("nexus-test-node");
    manager.ListVms();
    
    manager.StopVm("nexus-test-node");
    
    std::cout << "[+] VM Manager testing completed.\n";
    return 0;
}

#pragma once
#include "qemu_wrapper.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

struct NetworkInterface {
    std::string type; // "user" (NAT) or "tap" (Bridged)
    std::string mac_address;
    std::string bridge_name; // Used only if type == "tap"
};

struct VmConfig {
    std::string name;
    int memory_mb;
    int cpus;
    std::string architecture;
    std::vector<std::string> disks;
    std::vector<NetworkInterface> networks;
};

class VmManager {
private:
    std::unordered_map<std::string, VmConfig> vm_configs;
    std::unordered_map<std::string, std::shared_ptr<QemuWrapper>> active_vms;
    
    std::string GenerateRandomMac() const;

public:
    VmManager();

    bool CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch = "x86_64");
    bool AttachDisk(const std::string& vm_name, const std::string& disk_path);
    
    // Attaches a network interface to the VM (defaulting to NAT)
    bool AttachNetwork(const std::string& vm_name, const std::string& type = "user", const std::string& bridge = "");

    bool StartVm(const std::string& name);
    bool StopVm(const std::string& name);
    void ListVms() const;
};

#pragma once
#include "qemu_wrapper.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

struct VmConfig {
    std::string name;
    int memory_mb;
    int cpus;
    std::string architecture;
    std::vector<std::string> disks;
};

class VmManager {
private:
    std::unordered_map<std::string, VmConfig> vm_configs;
    std::unordered_map<std::string, std::shared_ptr<QemuWrapper>> active_vms;

public:
    VmManager();

    bool CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch = "x86_64");
    
    // Attaches a virtual disk path to a VM configuration
    bool AttachDisk(const std::string& vm_name, const std::string& disk_path);

    bool StartVm(const std::string& name);
    bool StopVm(const std::string& name);
    void ListVms() const;
};

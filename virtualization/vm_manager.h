#pragma once
#include "qemu_wrapper.h"
#include <string>
#include <memory>
#include <unordered_map>

struct VmConfig {
    std::string name;
    int memory_mb;
    int cpus;
    std::string architecture;
};

class VmManager {
private:
    std::unordered_map<std::string, VmConfig> vm_configs;
    std::unordered_map<std::string, std::shared_ptr<QemuWrapper>> active_vms;

public:
    VmManager();

    // Registers a new VM configuration
    bool CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch = "x86_64");

    // Constructs QEMU arguments and launches the VM via QemuWrapper
    bool StartVm(const std::string& name);

    // Gracefully stops an active VM
    bool StopVm(const std::string& name);

    // Displays all configured VMs and their runtime status
    void ListVms() const;
};

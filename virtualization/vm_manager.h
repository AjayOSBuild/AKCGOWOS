#pragma once
#include "qemu_wrapper.h"
#include "cgroup_manager.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

struct NetworkInterface {
    std::string type;
    std::string mac_address;
    std::string bridge_name;
};

struct VmConfig {
    std::string name;
    int memory_mb;
    int cpus;
    std::string architecture;
    std::vector<std::string> disks;
    std::vector<NetworkInterface> networks;
    
    // Resource Limits
    int cpu_quota_pct; // e.g., 50 for 50% of a core. 0 means unconstrained (beyond -smp)
    int hard_mem_limit_mb; // 0 means unconstrained
};

class VmManager {
private:
    std::unordered_map<std::string, VmConfig> vm_configs;
    std::unordered_map<std::string, std::shared_ptr<QemuWrapper>> active_vms;
    CgroupManager cgroup_mgr;
    
    std::string GenerateRandomMac() const;

public:
    VmManager();

    bool CreateVm(const std::string& name, int memory_mb, int cpus, const std::string& arch = "x86_64");
    bool AttachDisk(const std::string& vm_name, const std::string& disk_path);
    bool AttachNetwork(const std::string& vm_name, const std::string& type = "user", const std::string& bridge = "");
    
    // Sets hard cgroup resource limits
    bool SetResourceLimits(const std::string& vm_name, int cpu_quota_pct, int mem_limit_mb);

    bool StartVm(const std::string& name);
    bool StopVm(const std::string& name);
    void ListVms() const;
};

#include "cgroup_manager.h"
#include <iostream>
#include <fstream>

CgroupManager::CgroupManager() {
    std::cout << "[CgroupManager] Initializing Linux Control Group (cgroups v2) Manager...\n";
}

bool CgroupManager::ApplyLimits(const std::string& vm_name, pid_t pid, int cpu_quota_pct, int mem_limit_mb) const {
    std::cout << "[CgroupManager] Enforcing limits for VM '" << vm_name << "' (PID: " << pid << ")...\n";
    
    // Simulate writing to /sys/fs/cgroup/nexus-os/vms/<vm_name>
    std::string cgroup_path = "/sys/fs/cgroup/nexus-os/vms/" + vm_name;
    
    std::cout << "  -> [Mock] mkdir -p " << cgroup_path << "\n";
    
    if (cpu_quota_pct > 0) {
        int quota = (cpu_quota_pct * 100000) / 100;
        std::cout << "  -> [Mock] echo '" << quota << " 100000' > " << cgroup_path << "/cpu.max\n";
    }
    
    if (mem_limit_mb > 0) {
        long long mem_bytes = static_cast<long long>(mem_limit_mb) * 1024 * 1024;
        std::cout << "  -> [Mock] echo '" << mem_bytes << "' > " << cgroup_path << "/memory.max\n";
    }
    
    std::cout << "  -> [Mock] echo '" << pid << "' > " << cgroup_path << "/cgroup.procs\n";
    std::cout << "[CgroupManager] Resource limits applied successfully.\n";
    
    return true;
}

bool CgroupManager::RemoveLimits(const std::string& vm_name) const {
    std::string cgroup_path = "/sys/fs/cgroup/nexus-os/vms/" + vm_name;
    std::cout << "[CgroupManager] Removing cgroup limits for VM '" << vm_name << "'...\n";
    std::cout << "  -> [Mock] rmdir " << cgroup_path << "\n";
    return true;
}

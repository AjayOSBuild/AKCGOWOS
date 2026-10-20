#pragma once
#include <string>
#include <sys/types.h>

class CgroupManager {
public:
    CgroupManager();

    // Creates a cgroup for the VM and applies CPU/Memory limits
    bool ApplyLimits(const std::string& vm_name, pid_t pid, int cpu_quota_pct, int mem_limit_mb) const;

    // Cleans up the cgroup when the VM is destroyed
    bool RemoveLimits(const std::string& vm_name) const;
};

#pragma once
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct ResourceLimits {
    long memory_max_bytes = 512 * 1024 * 1024; // 512 MB default
    int cpu_weight = 100;                     // Cgroups v2 CPU weight (1-10000)
    int cpu_max_quota_us = 100000;            // 100ms quota per period
    int cpu_max_period_us = 100000;           // 100ms period
};

class CgroupsManager {
private:
    std::string cgroup_root;
    std::string container_cgroup_path;

public:
    CgroupsManager(const std::string& container_name, const std::string& base_path = "/sys/fs/cgroup/nexus");
    
    // Initializes the cgroups v2 directory slice
    bool InitializeCgroup();
    
    // Applies CPU and Memory resource constraints
    bool ApplyLimits(const ResourceLimits& limits);
    
    // Assigns a process PID to the container's cgroup control group
    bool AddProcess(int pid);
    
    // Removes the cgroup slice upon container teardown
    void Cleanup();
};

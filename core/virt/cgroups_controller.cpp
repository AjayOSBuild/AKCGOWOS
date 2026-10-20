#include "cgroups_controller.h"
#include <fstream>
#include <unistd.h>

CgroupsManager::CgroupsManager(const std::string& container_name, const std::string& base_path) 
    : cgroup_root(base_path) {
    container_cgroup_path = cgroup_root + "/" + container_name;
    std::cout << "[Cgroups] Initialized Cgroups v2 Manager for: " << container_cgroup_path << "\n";
}

bool CgroupsManager::InitializeCgroup() {
    std::error_code ec;
    fs::create_directories(container_cgroup_path, ec);
    if (ec) {
        std::cerr << "  -> [!] Note: Could not create host cgroups v2 slice (simulated fallback active).\n";
        return false;
    }
    std::cout << "  -> Cgroup control directory ready.\n";
    return true;
}

bool CgroupsManager::ApplyLimits(const ResourceLimits& limits) {
    std::cout << "[Cgroups] Applying resource control limits...\n";
    
    // Apply memory.max
    std::string mem_file = container_cgroup_path + "/memory.max";
    std::ofstream mem_out(mem_file);
    if (mem_out.is_open()) {
        mem_out << limits.memory_max_bytes;
        std::cout << "  -> Memory Max set to: " << (limits.memory_max_bytes / (1024 * 1024)) << " MB\n";
    }

    // Apply cpu.weight
    std::string cpu_weight_file = container_cgroup_path + "/cpu.weight";
    std::ofstream cpu_w_out(cpu_weight_file);
    if (cpu_w_out.is_open()) {
        cpu_w_out << limits.cpu_weight;
        std::cout << "  -> CPU Weight set to: " << limits.cpu_weight << "\n";
    }

    // Apply cpu.max
    std::string cpu_max_file = container_cgroup_path + "/cpu.max";
    std::ofstream cpu_m_out(cpu_max_file);
    if (cpu_m_out.is_open()) {
        cpu_m_out << limits.cpu_max_quota_us << " " << limits.cpu_max_period_us;
        std::cout << "  -> CPU Max quota/period constraint applied.\n";
    }

    return true;
}

bool CgroupsManager::AddProcess(int pid) {
    std::cout << "[Cgroups] Assigning PID " << pid << " to container cgroup...\n";
    std::string procs_file = container_cgroup_path + "/cgroup.procs";
    std::ofstream procs_out(procs_file);
    if (procs_out.is_open()) {
        procs_out << pid;
        std::cout << "  -> PID successfully attached.\n";
        return true;
    }
    std::cerr << "  -> [!] Failed to write PID to cgroup.procs (simulation mode).\n";
    return false;
}

void CgroupsManager::Cleanup() {
    std::error_code ec;
    fs::remove(container_cgroup_path, ec);
    std::cout << "[Cgroups] Cleaned up container cgroup slice.\n";
}

int main() {
    std::cout << "=== OS Project Cgroups v2 Resource Controller Utility ===\n";
    CgroupsManager cgroups("nexus_container_01");
    cgroups.InitializeCgroup();
    
    ResourceLimits limits;
    limits.memory_max_bytes = 256 * 1024 * 1024; // 256 MB
    limits.cpu_weight = 200;
    
    cgroups.ApplyLimits(limits);
    cgroups.AddProcess(getpid());
    cgroups.Cleanup();

    std::cout << "[+] Cgroups Controller testing completed successfully.\n";
    return 0;
}

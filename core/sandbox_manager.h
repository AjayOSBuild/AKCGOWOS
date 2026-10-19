#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <system_error>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>

namespace fs = std::filesystem;

class SandboxManager {
private:
    std::string app_id;
    std::string cgroup_path;

public:
    SandboxManager(const std::string& id) : app_id(id) {
        cgroup_path = "/sys/fs/cgroup/nexus_os_" + app_id;
    }

    bool InitializeSandboxEnvironment() {
        std::cout << "[SandboxManager] Preparing isolation container for: " << app_id << "\n";

        // 1. Setup Cgroups v2 limits
        std::error_code ec;
        if (!fs::exists(cgroup_path)) {
            fs::create_directories(cgroup_path, ec);
            if (ec) {
                std::cerr << "[!] Failed to create cgroup directory (Are cgroups v2 mounted?): " << ec.message() << "\n";
                // Non-fatal on systems without active cgroup v2 mounts in mock test environments
            } else {
                std::cout << "  -> Created cgroup partition at " << cgroup_path << "\n";
            }
        }

        // Write memory limit (e.g., 512MB) and CPU weight
        WriteCgroupParam("memory.max", "536870912"); 
        WriteCgroupParam("cpu.weight", "100");

        return true;
    }

    bool ApplyNamespaces() {
        std::cout << "[SandboxManager] Requesting unshare() for application process...\n";
        
        // Isolate namespaces: Mount, PID, IPC, Network, UTS
        int flags = CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWUTS;
        
        if (unshare(flags) < 0) {
            std::cerr << "[!] unshare() failed. Insufficient privileges or missing kernel capabilities.\n";
            return false;
        }

        std::cout << "  -> Successfully detached into isolated namespaces (PID, Mount, IPC, Net, UTS).\n";
        return true;
    }

    void AssignProcessToCgroup(pid_t pid) {
        std::string proc_path = cgroup_path + "/cgroup.procs";
        std::ofstream file(proc_path);
        if (file.is_open()) {
            file << pid;
            file.close();
            std::cout << "[SandboxManager] Assigned PID " << pid << " to cgroup " << cgroup_path << "\n";
        }
    }

private:
    void WriteCgroupParam(const std::string& param, const std::string& value) {
        std::string full_path = cgroup_path + "/" + param;
        std::ofstream file(full_path);
        if (file.is_open()) {
            file << value;
            file.close();
        }
    }
};

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class SecurityProfileGenerator {
private:
    std::string profile_dir;

public:
    SecurityProfileGenerator(const std::string& baseDir = "/etc/os/security/profiles") 
        : profile_dir(baseDir) {
        std::error_code ec;
        fs::create_directories(profile_dir, ec);
    }

    bool GenerateAppArmorProfile(const std::string& app_id, const std::vector<std::string>& allowed_paths) {
        std::string profile_path = (fs::path(profile_dir) / (app_id + ".profile")).string();
        std::ofstream file(profile_path);
        
        if (!file.is_open()) {
            std::cerr << "[!] Failed to create AppArmor profile for: " << app_id << "\n";
            return false;
        }

        file << "# AppArmor confinement profile for " << app_id << "\n";
        file << "# Generated automatically by Nexus OS Security Subsystem\n\n";
        file << "profile " << app_id << " flags=(attach_disconnected,mediate_deleted) {\n";
        file << "  # Standard system libraries and runtime access\n";
        file << "  /lib/** r,\n";
        file << "  /usr/lib/** r,\n";
        file << "  /proc/*/stat r,\n\n";
        file << "  # Application specific allowed paths\n";
        
        for (const auto& path : allowed_paths) {
            file << "  " << path << " rw,\n";
        }

        file << "  # Deny arbitrary root access\n";
        file << "  denied /root/** rwx,\n";
        file << "}\n";
        file.close();

        std::cout << "[SecurityProfile] Generated AppArmor profile at: " << profile_path << "\n";
        return true;
    }

    bool GenerateSeccompFilter(const std::string& app_id) {
        std::string filter_path = (fs::path(profile_dir) / (app_id + ".seccomp")).string();
        std::ofstream file(filter_path);

        if (!file.is_open()) {
            std::cerr << "[!] Failed to create Seccomp filter for: " << app_id << "\n";
            return false;
        }

        file << "{\n";
        file << "  \"default_action\": \"SCMP_ACT_KILL\",\n";
        file << "  \"architectures\": [\n    \"SCMP_ARCH_X86_64\"\n  ],\n";
        file << "  \"syscalls\": [\n";
        file << "    { \"names\": [\"read\", \"write\", \"exit\", \"rt_sigreturn\", \"mmap\", \"munmap\", \"poll\", \"select\"], \"action\": \"SCMP_ACT_ALLOW\" }\n";
        file << "  ]\n";
        file << "}\n";
        file.close();

        std::cout << "[SecurityProfile] Generated Seccomp-BPF syscall whitelist filter at: " << filter_path << "\n";
        return true;
    }
};

#pragma once
#include "package_format.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class PackageRegistry {
private:
    std::string registry_dir;
    std::unordered_map<std::string, NexusPackageMetadata> installed_packages;

    void LoadRegistry() {
        // In full execution, this parses a JSON/TOML database file.
        // For scaffold, we assume an empty registry on cold boot.
    }

    void SaveRegistry() {
        std::error_code ec;
        fs::create_directories(registry_dir, ec);
        
        std::string db_file = registry_dir + "/installed.dat";
        std::ofstream out(db_file);
        if (out.is_open()) {
            for (const auto& pair : installed_packages) {
                out << pair.second.name << "|" << pair.second.version << "|" 
                    << pair.second.architecture << "|" << pair.second.maintainer << "\n";
            }
        }
    }

public:
    PackageRegistry(const std::string& db_path = "/var/lib/nexus_os/pkg") : registry_dir(db_path) {
        std::cout << "[PkgRegistry] Initializing Package Metadata Registry at " << registry_dir << "\n";
        LoadRegistry();
    }

    bool RegisterPackage(const NexusPackageMetadata& meta) {
        if (!meta.is_valid || meta.name.empty()) {
            std::cerr << "[!] Cannot register invalid package metadata.\n";
            return false;
        }

        installed_packages[meta.name] = meta;
        SaveRegistry();
        std::cout << "[PkgRegistry] Registered package: " << meta.name << " v" << meta.version << "\n";
        return true;
    }

    bool UnregisterPackage(const std::string& name) {
        auto it = installed_packages.find(name);
        if (it != installed_packages.end()) {
            installed_packages.erase(it);
            SaveRegistry();
            std::cout << "[PkgRegistry] Unregistered package: " << name << "\n";
            return true;
        }
        std::cerr << "[!] Package not found in registry: " << name << "\n";
        return false;
    }

    bool IsInstalled(const std::string& name) const {
        return installed_packages.find(name) != installed_packages.end();
    }

    NexusPackageMetadata GetMetadata(const std::string& name) const {
        auto it = installed_packages.find(name);
        if (it != installed_packages.end()) {
            return it->second;
        }
        return {"", "", "", "", "", false};
    }

    std::vector<NexusPackageMetadata> ListInstalled() const {
        std::vector<NexusPackageMetadata> list;
        for (const auto& pair : installed_packages) {
            list.push_back(pair.second);
        }
        return list;
    }
};

#pragma once
#include "package_metadata.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class PackageUninstaller {
private:
    PackageRegistry& registry;

public:
    PackageUninstaller(PackageRegistry& reg) : registry(reg) {
        std::cout << "[PkgUninstaller] Package Uninstaller Engine initialized.\n";
    }

    bool UninstallPackage(const std::string& package_name) {
        std::cout << "[PkgUninstaller] Initiating uninstallation for: " << package_name << "\n";
        
        // 1. Verify package exists in the registry
        if (!registry.IsInstalled(package_name)) {
            std::cerr << "[!] Uninstallation aborted: Package '" << package_name << "' is not installed.\n";
            return false;
        }

        // 2. Remove payload files from the filesystem
        // We use the safe sandbox target `/tmp/os_root` established in PKG-005
        std::string install_path = "/tmp/os_root/opt/" + package_name; 
        std::error_code ec;
        if (fs::exists(install_path)) {
            fs::remove_all(install_path, ec);
            if (ec) {
                std::cerr << "[!] Warning: Failed to cleanly remove all files at " << install_path << "\n";
            } else {
                std::cout << "  -> Removed application files at " << install_path << "\n";
            }
        } else {
            std::cout << "  -> (Simulated) Removed application files from system paths.\n";
        }

        // 3. Remove package from metadata database
        if (!registry.UnregisterPackage(package_name)) {
            std::cerr << "[!] Uninstallation error: Failed to unregister package metadata.\n";
            return false;
        }

        std::cout << "[+] Successfully uninstalled package: " << package_name << "\n";
        return true;
    }
};

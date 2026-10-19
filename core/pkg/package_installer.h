#pragma once
#include "package_format.h"
#include "package_metadata.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class PackageInstaller {
private:
    PackageRegistry& registry;

public:
    PackageInstaller(PackageRegistry& reg) : registry(reg) {
        std::cout << "[PkgInstaller] Package Installer Engine initialized.\n";
    }

    bool InstallPackage(const std::string& npk_path) {
        std::cout << "[PkgInstaller] Initiating installation for: " << npk_path << "\n";
        
        PackageFormatHandler format_handler(npk_path);
        
        // 1. Validate signature
        if (!format_handler.ValidateSignature()) {
            std::cerr << "[!] Installation aborted: Invalid package signature.\n";
            return false;
        }

        // 2. Extract and parse metadata
        NexusPackageMetadata meta = format_handler.ExtractManifest();
        if (!meta.is_valid) {
            std::cerr << "[!] Installation aborted: Corrupt or missing manifest.toml.\n";
            return false;
        }

        // 3. Check if already installed
        if (registry.IsInstalled(meta.name)) {
            std::cout << "[PkgInstaller] Package '" << meta.name << "' is already installed.\n";
            // Note: In full execution, an upgrade workflow would trigger here.
            return true;
        }

        // 4. Extract payload to the root filesystem (simulated)
        // We use a safe sandbox target `/tmp/os_root` for testing to prevent host pollution
        std::string target_root = "/tmp/os_root"; 
        if (!format_handler.ExtractPayload(target_root)) {
            std::cerr << "[!] Installation aborted: Payload extraction failed.\n";
            return false;
        }

        // 5. Register with the system metadata database
        if (!registry.RegisterPackage(meta)) {
            std::cerr << "[!] Installation aborted: Failed to register package metadata.\n";
            return false;
        }

        std::cout << "[+] Successfully installed package: " << meta.name << " v" << meta.version << "\n";
        return true;
    }
};

#pragma once
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

template <typename InstallerType, typename UninstallerType>
class PackageRollback {
private:
    std::string snapshot_dir;
    InstallerType& installer;
    UninstallerType& uninstaller;

public:
    PackageRollback(InstallerType& inst, UninstallerType& uninst, const std::string& cache_dir = "/var/cache/nexus_os/rollback")
        : installer(inst), uninstaller(uninst), snapshot_dir(cache_dir) {
        std::cout << "[PkgRollback] Package Rollback Engine initialized at: " << snapshot_dir << "\n";
        std::error_code ec;
        fs::create_directories(snapshot_dir, ec);
    }

    bool CreateSnapshot(const std::string& pkg_name, const std::string& old_npk_path) {
        std::cout << "[PkgRollback] Creating rollback snapshot for: " << pkg_name << "...\n";
        std::error_code ec;
        
        if (!fs::exists(old_npk_path)) {
            std::cerr << "[!] Cannot create snapshot: Source package missing (" << old_npk_path << ").\n";
            return false;
        }

        std::string target = snapshot_dir + "/" + pkg_name + ".npk";
        fs::copy(old_npk_path, target, fs::copy_options::overwrite_existing, ec);
        
        if (ec) {
            std::cerr << "[!] Failed to cache snapshot: " << ec.message() << "\n";
            return false;
        }
        
        std::cout << "  -> Snapshot securely cached.\n";
        return true;
    }

    bool PerformRollback(const std::string& pkg_name) {
        std::string snapshot_file = snapshot_dir + "/" + pkg_name + ".npk";
        if (!fs::exists(snapshot_file)) {
            std::cerr << "[!] Rollback aborted: No cached snapshot available for '" << pkg_name << "'.\n";
            return false;
        }
        
        std::cout << "\n[PkgRollback] Initiating rollback sequence for: " << pkg_name << "\n";
        
        // 1. Uninstall current broken version
        std::cout << "  -> Step 1: Removing current version...\n";
        if (!uninstaller.UninstallPackage(pkg_name)) {
            std::cerr << "[!] Rollback failed during uninstallation phase.\n";
            return false;
        }
        
        // 2. Reinstall cached version
        std::cout << "  -> Step 2: Restoring from snapshot...\n";
        if (!installer.InstallPackage(snapshot_file)) {
            std::cerr << "[!] CRITICAL: Rollback failed during restoration phase.\n";
            return false;
        }
        
        std::cout << "[+] Rollback completed successfully. System state restored.\n";
        return true;
    }
};

#pragma once
#include <iostream>
#include <string>
#include <vector>

struct UpdateCandidate {
    std::string package_name;
    std::string current_version;
    std::string new_version;
    std::string download_url;
};

template <typename RegistryType, typename RepoType, typename InstallerType>
class PackageUpdater {
private:
    RegistryType& registry;
    RepoType& repo;
    InstallerType& installer;

    // Semantic version string comparison logic
    bool IsNewerVersion(const std::string& current, const std::string& remote) {
        // Simplified semantic versioning check for scaffold (e.g. "2.1.4" vs "2.1.5")
        // In full execution, this tokenizes by '.' and parses as integers.
        return remote > current; 
    }

public:
    PackageUpdater(RegistryType& reg, RepoType& r, InstallerType& inst) 
        : registry(reg), repo(r), installer(inst) {
        std::cout << "[PkgUpdater] Package Updater Engine initialized.\n";
    }

    std::vector<UpdateCandidate> CheckForUpdates() {
        std::cout << "[PkgUpdater] Checking for software updates...\n";
        std::vector<UpdateCandidate> updates;

        auto installed = registry.ListInstalled();
        for (const auto& pkg : installed) {
            // We need a generic way to retrieve the RemotePackageInfo struct
            // Since this is a template, we use auto or assume RemotePackageInfo is accessible.
            // For simplicity, we rely on duck typing provided by the templates.
            if (repo.HasNewerVersion(pkg.name, pkg.version)) {
                auto remote_info = repo.GetPackageInfo(pkg.name);
                updates.push_back({pkg.name, pkg.version, remote_info.version, remote_info.url});
            }
        }

        std::cout << "[PkgUpdater] Found " << updates.size() << " available updates.\n";
        return updates;
    }

    bool ApplyUpdate(const UpdateCandidate& candidate) {
        std::cout << "[PkgUpdater] Updating '" << candidate.package_name 
                  << "' from v" << candidate.current_version 
                  << " to v" << candidate.new_version << "...\n";
        
        // 1. Simulate download
        std::cout << "  -> Downloading from " << candidate.download_url << "...\n";
        std::string downloaded_pkg = "/tmp/" + candidate.package_name + "_update.npk";
        
        // 2. Pass to installer
        return installer.InstallPackage(downloaded_pkg);
    }

    void UpdateAll() {
        auto updates = CheckForUpdates();
        if (updates.empty()) {
            std::cout << "[+] System is fully up to date.\n";
            return;
        }

        for (const auto& update : updates) {
            ApplyUpdate(update);
        }
        std::cout << "[+] System update cycle completed.\n";
    }
};

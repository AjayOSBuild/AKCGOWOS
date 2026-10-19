#include <iostream>
#include <vector>
#include "package_format.h"
#include "repository_format.h"
#include "package_updater.h"

// --- Mock Classes for Testing ---
class MockRegistryU {
public:
    struct MockMeta { std::string name; std::string version; };
    std::vector<MockMeta> ListInstalled() {
        return {{"nexus-browser", "2.1.4"}, {"coreutils", "8.32"}};
    }
};

class MockRepoU {
public:
    struct MockRemoteInfo { std::string version; std::string url; };
    
    bool HasNewerVersion(const std::string& name, const std::string& current_ver) {
        if (name == "nexus-browser") return "2.1.5" > current_ver;
        return false; // coreutils stays at 8.32
    }
    
    MockRemoteInfo GetPackageInfo(const std::string& name) {
        if (name == "nexus-browser") return {"2.1.5", "https://repo.nexus-os.local/nexus-browser-2.1.5.npk"};
        return {"", ""};
    }
};

class MockInstallerU {
public:
    bool InstallPackage(const std::string& npk_path) {
        std::cout << "  -> [MockInstaller] Extracting and installing package from " << npk_path << "...\n";
        std::cout << "  -> [MockInstaller] Old version replaced atomically. Registry updated.\n";
        return true;
    }
};

int main() {
    std::cout << "=== OS Project Package Updater Utility ===\n";
    
    MockRegistryU reg;
    MockRepoU repo;
    MockInstallerU inst;

    PackageUpdater<MockRegistryU, MockRepoU, MockInstallerU> updater(reg, repo, inst);

    std::cout << "\n--- Simulating System Update Cycle ---\n";
    updater.UpdateAll();

    std::cout << "\n[+] Package Updater testing completed successfully.\n";
    return 0;
}

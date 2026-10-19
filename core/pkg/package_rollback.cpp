#include <iostream>
#include <fstream>
#include "package_rollback.h"

// --- Mock Classes for Testing ---
class MockInstallerR {
public:
    bool InstallPackage(const std::string& path) {
        std::cout << "      [MockInstaller] Restoring package payload from " << path << "...\n";
        std::cout << "      [MockInstaller] Registry metadata updated.\n";
        return true;
    }
};

class MockUninstallerR {
public:
    bool UninstallPackage(const std::string& name) {
        std::cout << "      [MockUninstaller] Purging application files for " << name << "...\n";
        return true;
    }
};

int main() {
    std::cout << "=== OS Project Package Rollback Utility ===\n";
    
    MockInstallerR inst;
    MockUninstallerR uninst;
    PackageRollback<MockInstallerR, MockUninstallerR> rollback(inst, uninst, "/tmp/rollback_cache");

    // Create a dummy .npk file to simulate an old cached version
    std::string dummy_old_pkg = "/tmp/old_nexus_browser.npk";
    std::ofstream dummy(dummy_old_pkg);
    dummy << "OLD_VERSION_DATA";
    dummy.close();

    // Simulate pre-update snapshot creation
    std::cout << "\n--- Simulating Snapshot Creation (Pre-Update) ---\n";
    rollback.CreateSnapshot("nexus-browser", dummy_old_pkg);

    // Simulate rollback triggered by a user after a bad update
    std::cout << "\n--- Simulating Emergency Rollback ---\n";
    rollback.PerformRollback("nexus-browser");

    std::cout << "\n[+] Package Rollback testing completed successfully.\n";
    return 0;
}

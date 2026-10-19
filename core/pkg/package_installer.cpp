#include <iostream>
#include <fstream>
#include "package_metadata.h"
#include "package_installer.h"

int main() {
    std::cout << "=== OS Project Package Installer Utility ===\n";
    
    PackageRegistry registry;
    PackageInstaller installer(registry);

    // Create a dummy .npk file for testing
    std::string dummy_pkg = "/tmp/nexus-calculator-1.0.npk";
    std::ofstream dummy(dummy_pkg);
    dummy << "DUMMY_NPK_ARCHIVE_DATA";
    dummy.close();

    // Simulate installation workflow
    std::cout << "\n--- Testing Package Installation ---\n";
    installer.InstallPackage(dummy_pkg);

    std::cout << "\n[+] Package Installer testing completed successfully.\n";
    return 0;
}

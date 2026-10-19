#include <iostream>
#include "package_metadata.h"
#include "package_uninstaller.h"

int main() {
    std::cout << "=== OS Project Package Uninstaller Utility ===\n";
    
    PackageRegistry registry;
    PackageUninstaller uninstaller(registry);

    // Simulate pre-existing installed package
    NexusPackageMetadata sample_pkg;
    sample_pkg.name = "nexus-calculator";
    sample_pkg.version = "1.0.0";
    sample_pkg.architecture = "x86_64";
    sample_pkg.maintainer = "Nexus OS Team";
    sample_pkg.is_valid = true;
    
    if (!registry.IsInstalled("nexus-calculator")) {
        registry.RegisterPackage(sample_pkg);
    }

    // Simulate uninstallation workflow
    std::cout << "\n--- Testing Package Uninstallation ---\n";
    uninstaller.UninstallPackage("nexus-calculator");

    std::cout << "\n[+] Package Uninstaller testing completed successfully.\n";
    return 0;
}

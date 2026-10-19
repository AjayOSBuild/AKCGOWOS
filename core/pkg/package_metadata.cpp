#include <iostream>
#include "package_metadata.h"

int main() {
    std::cout << "=== OS Project Package Metadata Registry ===\n";
    PackageRegistry registry;

    // Simulate extracting metadata from a new package and registering it
    NexusPackageMetadata sample_pkg;
    sample_pkg.name = "nexus-browser";
    sample_pkg.version = "2.1.4";
    sample_pkg.architecture = "x86_64";
    sample_pkg.maintainer = "Nexus OS Team";
    sample_pkg.description = "Native web browser for Nexus OS.";
    sample_pkg.is_valid = true;

    // Test Registration
    registry.RegisterPackage(sample_pkg);

    // Test Query
    if (registry.IsInstalled("nexus-browser")) {
        NexusPackageMetadata retrieved = registry.GetMetadata("nexus-browser");
        std::cout << "[Test] Query successful. Retrieved version: " << retrieved.version << "\n";
    }

    // Test Unregistration
    registry.UnregisterPackage("nexus-browser");

    std::cout << "[+] Package Metadata Registry testing completed successfully.\n";
    return 0;
}

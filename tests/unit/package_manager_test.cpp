#include "../test_framework.h"
#include <string>
#include <vector>

// Mock package manager abstraction functions for testing
int CompareVersions(const std::string& v1, const std::string& v2) {
    // Simplified version comparison (e.g., "1.0.0" vs "2.0.0")
    if (v1 == v2) return 0;
    return (v1 < v2) ? -1 : 1; // Basic string comparison for mock purposes
}

bool IsValidPackageArchive(const std::string& filename) {
    // Check if filename ends with our supported package extensions
    return (filename.find(".nexpkg") != std::string::npos || 
            filename.find(".tar.gz") != std::string::npos);
}

TEST(Pkg_VersionComparison) {
    ASSERT_EQ(0, CompareVersions("1.2.3", "1.2.3"));
    ASSERT_EQ(-1, CompareVersions("1.0.0", "2.0.0"));
    ASSERT_EQ(1, CompareVersions("2.1.0", "1.9.9"));
}

TEST(Pkg_ArchiveValidation) {
    ASSERT_TRUE(IsValidPackageArchive("core-utils-1.0.nexpkg"));
    ASSERT_TRUE(IsValidPackageArchive("lib-network.tar.gz"));
    ASSERT_EQ(false, IsValidPackageArchive("malicious_script.sh"));
    ASSERT_EQ(false, IsValidPackageArchive("unsupported.deb"));
}

int main() {
    return TestRegistry::RunAll();
}

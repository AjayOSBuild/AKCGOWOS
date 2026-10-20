#include "../test_framework.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cctype>

// Mock Android compatibility abstraction functions for testing
bool IsValidApkSignature(const std::vector<uint8_t>& header) {
    if (header.size() < 4) return false;
    // APKs are packaged as ZIP files; check for standard ZIP magic number 'PK\x03\x04'
    return header[0] == 0x50 && header[1] == 0x4B && header[2] == 0x03 && header[3] == 0x04;
}

bool IsValidPackageName(const std::string& pkg) {
    // Basic Android package name validation: two or more dot-separated segments, starting with a letter
    if (pkg.length() < 3 || pkg.find('.') == std::string::npos) return false;
    return std::isalpha(pkg[0]);
}

TEST(Android_ApkDetection) {
    // Valid ZIP/APK header
    ASSERT_TRUE(IsValidApkSignature({0x50, 0x4B, 0x03, 0x04, 0x14, 0x00}));
    
    // Invalid (Linux ELF header)
    ASSERT_EQ(false, IsValidApkSignature({0x7F, 0x45, 0x4C, 0x46}));
    
    // Empty buffer
    ASSERT_EQ(false, IsValidApkSignature({}));
}

TEST(Android_PackageNameValidation) {
    ASSERT_TRUE(IsValidPackageName("com.nexus.app"));
    ASSERT_TRUE(IsValidPackageName("org.example.game"));
    ASSERT_EQ(false, IsValidPackageName("app")); // missing dot
    ASSERT_EQ(false, IsValidPackageName("1com.app")); // starts with number
}

int main() {
    return TestRegistry::RunAll();
}

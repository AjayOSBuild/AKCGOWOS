#include "../test_framework.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cctype>

// Mock Windows compatibility abstraction functions for testing
std::string WinPathToPosix(const std::string& win_path) {
    std::string posix_path = "";
    if (win_path.length() >= 2 && win_path[1] == ':') {
        posix_path += "/mnt/";
        posix_path += std::tolower(win_path[0]);
        for (size_t i = 2; i < win_path.length(); ++i) {
            if (win_path[i] == '\\') posix_path += "/";
            else posix_path += win_path[i];
        }
    }
    return posix_path;
}

bool IsValidExeSignature(const std::vector<uint8_t>& header) {
    if (header.size() < 2) return false;
    // Check for standard DOS 'MZ' executable magic number
    return header[0] == 0x4D && header[1] == 0x5A;
}

TEST(WinCompat_PathConversion) {
    ASSERT_EQ("/mnt/c/Windows/System32/cmd.exe", WinPathToPosix("C:\\Windows\\System32\\cmd.exe"));
    ASSERT_EQ("/mnt/d/Games/app.exe", WinPathToPosix("D:\\Games\\app.exe"));
}

TEST(WinCompat_ExeDetection) {
    // Valid MZ header
    ASSERT_TRUE(IsValidExeSignature({0x4D, 0x5A, 0x90, 0x00}));
    
    // Invalid (Linux ELF header)
    ASSERT_EQ(false, IsValidExeSignature({0x7F, 0x45, 0x4C, 0x46}));
    
    // Empty buffer
    ASSERT_EQ(false, IsValidExeSignature({}));
}

int main() {
    return TestRegistry::RunAll();
}

#include "../test_framework.h"
#include <string>

// Mock filesystem abstraction functions for testing purposes
bool IsValidPath(const std::string& path) {
    return !path.empty() && path[0] == '/';
}

std::string GetExtension(const std::string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos) return "";
    return filename.substr(pos + 1);
}

TEST(FS_PathValidation) {
    ASSERT_TRUE(IsValidPath("/root/system"));
    ASSERT_TRUE(IsValidPath("/dev/vda1"));
    ASSERT_EQ(false, IsValidPath("relative/path"));
}

TEST(FS_ExtensionParsing) {
    ASSERT_EQ("exe", GetExtension("app.exe"));
    ASSERT_EQ("txt", GetExtension("document.txt"));
    ASSERT_EQ("", GetExtension("noextension"));
}

int main() {
    return TestRegistry::RunAll();
}

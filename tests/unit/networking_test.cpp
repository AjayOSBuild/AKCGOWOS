#include "../test_framework.h"
#include <string>
#include <regex>

// Mock networking abstraction functions for testing purposes
bool IsValidIPv4(const std::string& ip) {
    std::regex ipv4_regex("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$");
    return std::regex_match(ip, ipv4_regex);
}

std::string FormatMacAddress(const std::string& unformatted) {
    if (unformatted.length() != 12) return "";
    std::string formatted = "";
    for (size_t i = 0; i < 12; i += 2) {
        formatted += unformatted.substr(i, 2);
        if (i < 10) formatted += ":";
    }
    return formatted;
}

TEST(Net_IPv4Validation) {
    ASSERT_TRUE(IsValidIPv4("192.168.1.1"));
    ASSERT_TRUE(IsValidIPv4("0.0.0.0"));
    ASSERT_TRUE(IsValidIPv4("255.255.255.255"));
    
    ASSERT_EQ(false, IsValidIPv4("256.256.256.256"));
    ASSERT_EQ(false, IsValidIPv4("192.168.1"));
    ASSERT_EQ(false, IsValidIPv4("not.an.ip"));
}

TEST(Net_MacFormatting) {
    ASSERT_EQ("00:1A:2B:3C:4D:5E", FormatMacAddress("001A2B3C4D5E"));
    ASSERT_EQ("", FormatMacAddress("INVALID"));
}

int main() {
    return TestRegistry::RunAll();
}

#include "../test_framework.h"

TEST(Framework_BasicAssertionPass) {
    ASSERT_TRUE(true);
    ASSERT_EQ(42, 42);
}

TEST(Framework_StringEquality) {
    std::string a = "nexus";
    std::string b = "nexus";
    ASSERT_EQ(a, b);
}

int main() {
    return TestRegistry::RunAll();
}

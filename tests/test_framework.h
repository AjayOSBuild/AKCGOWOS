#pragma once
#include <iostream>
#include <string>
#include <vector>

// Minimal test registry and assertion macros to avoid heavy external dependencies
class TestRegistry {
public:
    struct TestCase {
        std::string name;
        void (*func)();
    };

    static std::vector<TestCase>& GetTests() {
        static std::vector<TestCase> tests;
        return tests;
    }

    static void Register(const std::string& name, void (*func)()) {
        GetTests().push_back({name, func});
    }

    static int RunAll() {
        int passed = 0;
        int failed = 0;
        std::cout << "[TestRunner] Executing " << GetTests().size() << " tests...\n";
        
        for (const auto& test : GetTests()) {
            try {
                test.func();
                std::cout << "[PASS] " << test.name << "\n";
                passed++;
            } catch (const std::exception& e) {
                std::cout << "[FAIL] " << test.name << " - " << e.what() << "\n";
                failed++;
            }
        }
        
        std::cout << "\n======================================\n";
        std::cout << "TEST RUN COMPLETE: " << passed << " Passed, " << failed << " Failed.\n";
        std::cout << "======================================\n";
        return failed == 0 ? 0 : 1;
    }
};

#define TEST(Name) \
    void Name(); \
    struct Register##Name { \
        Register##Name() { TestRegistry::Register(#Name, Name); } \
    } register_##Name; \
    void Name()

#define ASSERT_TRUE(condition) \
    if (!(condition)) throw std::runtime_error("Assertion failed: " #condition)

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) throw std::runtime_error("Assertion failed: " #expected " == " #actual)

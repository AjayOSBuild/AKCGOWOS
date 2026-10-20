#pragma once
#include "vm_manager.h"
#include <string>
#include <functional>

enum class TestResult {
    PASS,
    FAIL,
    TIMEOUT,
    CRASH
};

class VmTestEnvironment {
private:
    VmManager vm_mgr;
    std::string test_run_id;

public:
    VmTestEnvironment(const std::string& run_id);

    // Provisions a clean, ephemeral VM for a specific test suite
    bool ProvisionTestNode(const std::string& node_name, int memory_mb = 512, int cpus = 1);

    // Simulates executing a test payload inside the VM via guest agent or serial console
    TestResult ExecuteTest(const std::string& node_name, const std::string& test_command, int timeout_seconds = 30);

    // Destroys the test node and purges all ephemeral state
    bool TeardownTestNode(const std::string& node_name);
};

#include "vm_test_env.h"
#include <iostream>
#include <unistd.h>

VmTestEnvironment::VmTestEnvironment(const std::string& run_id) : test_run_id(run_id) {
    std::cout << "[TestEnv] Initializing Ephemeral VM Testing Environment (Run ID: " << test_run_id << ")...\n";
}

bool VmTestEnvironment::ProvisionTestNode(const std::string& node_name, int memory_mb, int cpus) {
    std::cout << "[TestEnv] Provisioning ephemeral test node: " << node_name << "\n";
    
    if (!vm_mgr.CreateVm(node_name, memory_mb, cpus)) {
        std::cerr << "[!] Failed to configure test node.\n";
        return false;
    }

    // Isolate the test node to prevent interference with the host network
    vm_mgr.SetResourceLimits(node_name, 100, memory_mb + 100);
    std::cout << "[TestEnv] Configured isolated test environment parameters.\n";

    if (!vm_mgr.StartVm(node_name)) {
        std::cerr << "[!] Failed to boot test node.\n";
        return false;
    }
    
    std::cout << "[TestEnv] Test node '" << node_name << "' is active and ready for payloads.\n";
    return true;
}

TestResult VmTestEnvironment::ExecuteTest(const std::string& node_name, const std::string& test_command, int timeout_seconds) {
    std::cout << "[TestEnv] Injecting test payload via guest serial agent...\n";
    std::cout << "  -> [Command] " << test_command << "\n";
    
    // Simulate test execution delay
    sleep(1);
    
    std::cout << "[TestEnv] Awaiting exit code (Timeout: " << timeout_seconds << "s)...\n";
    
    // Mocking evaluation logic
    if (test_command.find("crash") != std::string::npos) {
        std::cout << "  -> [Result] Kernel panic detected in guest!\n";
        return TestResult::CRASH;
    } else if (test_command.find("timeout") != std::string::npos) {
        std::cout << "  -> [Result] Execution exceeded " << timeout_seconds << "s.\n";
        return TestResult::TIMEOUT;
    }
    
    std::cout << "  -> [Result] Exit code 0.\n";
    return TestResult::PASS;
}

bool VmTestEnvironment::TeardownTestNode(const std::string& node_name) {
    std::cout << "[TestEnv] Initiating teardown for test node '" << node_name << "'...\n";
    if (vm_mgr.StopVm(node_name)) {
        std::cout << "[TestEnv] Test node successfully destroyed and resources reclaimed.\n";
        return true;
    }
    std::cerr << "[!] Failed to cleanly teardown test node '" << node_name << "'.\n";
    return false;
}

int main() {
    std::cout << "=== OS Project VM Testing Environment Utility ===\n";
    
    VmTestEnvironment test_env("RUN-90210");
    std::string target_node = "ci-worker-01";
    
    if (test_env.ProvisionTestNode(target_node)) {
        
        std::cout << "\n--- Running Valid Test Suite ---\n";
        TestResult res1 = test_env.ExecuteTest(target_node, "/usr/bin/run_fs_tests --all");
        std::cout << "Status: " << (res1 == TestResult::PASS ? "PASS" : "FAIL") << "\n";
        
        std::cout << "\n--- Running Destructive Test Suite ---\n";
        TestResult res2 = test_env.ExecuteTest(target_node, "echo c > /proc/sysrq-trigger # crash");
        std::cout << "Status: " << (res2 == TestResult::CRASH ? "CRASH" : "FAIL") << "\n\n";

        test_env.TeardownTestNode(target_node);
    }
    
    std::cout << "[+] VM Testing Environment check completed.\n";
    return 0;
}

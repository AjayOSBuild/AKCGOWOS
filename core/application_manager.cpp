#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "sandbox_manager.h"

int main() {
    std::cout << "=== OS Project Application Manager (Ring 3) ===\n";
    std::cout << "[*] Initializing application lifecycle and sandbox enforcement engine...\n";

    // Simulate launching an untrusted application with strict sandboxing
    std::string target_app = "com.untrusted.game";
    std::cout << "\n[ApplicationManager] Launch request received for: " << target_app << "\n";

    SandboxManager sandbox(target_app);
    
    if (sandbox.InitializeSandboxEnvironment()) {
        std::cout << "[ApplicationManager] Sandbox environment initialized successfully.\n";
        
        // In full execution, the ApplicationManager calls fork(), 
        // the child calls sandbox.ApplyNamespaces(), and the PID is assigned to cgroups.
        pid_t mock_spawned_pid = 4092;
        sandbox.AssignProcessToCgroup(mock_spawned_pid);
    }

    std::cout << "[+] Application Manager active. Enforcing sandbox boundaries for all child processes.\n";

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}

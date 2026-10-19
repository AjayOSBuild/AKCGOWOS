#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "security_policy.h"

int main() {
    std::cout << "=== OS Project Permission Manager (Ring 3) ===\n";
    std::cout << "[*] Initializing dynamic permission broker and access control lists...\n";

    SecurityPolicyEngine policy_engine;

    // Register default system apps and test packages
    policy_engine.RegisterApp("com.nexus.terminal", true);
    policy_engine.RegisterApp("com.nexus.browser", true);

    // Simulate an application requesting access to a protected resource (e.g., Camera)
    std::string requesting_app = "com.nexus.browser";
    PermissionScope target_scope = PermissionScope::CAMERA;

    std::cout << "\n[PermissionBroker] Intercepted resource request from [" << requesting_app << "] for CAMERA...\n";
    PermissionDecision decision = policy_engine.EvaluateRequest(requesting_app, target_scope);

    if (decision == PermissionDecision::PROMPT_USER) {
        std::cout << "[PermissionBroker] Access not explicitly granted. Prompting user for consent...\n";
        // Simulate user clicking "Allow"
        policy_engine.GrantPermission(requesting_app, target_scope, PermissionDecision::ALLOW);
    }

    std::cout << "[+] Permission Manager active and actively brokering IPC access requests.\n";

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}

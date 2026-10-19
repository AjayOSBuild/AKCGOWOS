#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

enum class PermissionScope {
    CAMERA,
    MICROPHONE,
    LOCATION,
    STORAGE_EXTERNAL,
    NETWORK_INTERNET
};

enum class PermissionDecision {
    DENY = 0,
    ALLOW = 1,
    PROMPT_USER = 2
};

struct AppSecurityProfile {
    std::string app_id;
    bool is_sandboxed = true;
    std::unordered_map<int, PermissionDecision> explicit_grants;
};

class SecurityPolicyEngine {
private:
    std::unordered_map<std::string, AppSecurityProfile> app_profiles;

public:
    SecurityPolicyEngine() {
        std::cout << "[SecurityPolicy] Initializing mandatory access control (MAC) policies...\n";
    }

    void RegisterApp(const std::string& app_id, bool sandboxed) {
        app_profiles[app_id] = {app_id, sandboxed, {}};
        std::cout << "[SecurityPolicy] Registered application profile: " << app_id << " (Sandboxed: " << (sandboxed ? "Yes" : "No") << ")\n";
    }

    PermissionDecision EvaluateRequest(const std::string& app_id, PermissionScope scope) {
        auto it = app_profiles.find(app_id);
        if (it == app_profiles.end()) {
            // Default policy for unknown/unregistered apps: Prompt or Deny
            return PermissionDecision.PROMPT_USER;
        }

        int scope_key = static_cast<int>(scope);
        if (it->second.explicit_grants.find(scope_key) != it->second.explicit_grants.end()) {
            return it->second.explicit_grants[scope_key];
        }

        // Default scopes require user prompt
        return PermissionDecision.PROMPT_USER;
    }

    void GrantPermission(const std::string& app_id, PermissionScope scope, PermissionDecision decision) {
        if (app_profiles.find(app_id) != app_profiles.end()) {
            app_profiles[app_id].explicit_grants[static_cast<int>(scope)] = decision;
            std::cout << "[SecurityPolicy] Updated grant for " << app_id << " -> Scope " << static_cast<int>(scope) 
                      << " : " << (decision == PermissionDecision.ALLOW ? "ALLOW" : "DENY") << "\n";
        }
    }
};

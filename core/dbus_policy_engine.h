#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

enum class DBusAccessDecision {
    ALLOW = 0,
    DENY = 1
};

struct DBusRule {
    std::string interface_name;
    std::string method_name;
    bool allow_unprivileged = false;
};

class DBusPolicyEngine {
private:
    std::vector<DBusRule> restricted_rules;

public:
    DBusPolicyEngine() {
        std::cout << "[DBusPolicy] Initializing IPC security policy enforcement engine...\n";
        
        // Define privileged system management methods restricted from unprivileged apps
        restricted_rules.push_back({"os.core.ServiceManager", "RestartService", false});
        restricted_rules.push_back({"os.core.StorageManager", "Mount", false});
        restricted_rules.push_back({"os.core.StorageManager", "Unmount", false});
        restricted_rules.push_back({"os.core.SystemEventService", "Shutdown", false});
        restricted_rules.push_back({"os.core.SystemEventService", "Reboot", false});
    }

    DBusAccessDecision EvaluateMessage(uint32_t sender_uid, const std::string& interface, const std::string& method) {
        // Root (UID 0) and core system daemons (UID < 1000) have full privileges
        if (sender_uid == 0 || sender_uid < 1000) {
            return DBusAccessDecision::ALLOW;
        }

        // Evaluate against restricted rules database
        for (const auto& rule : restricted_rules) {
            if (rule.interface_name == interface && rule.method_name == method) {
                if (!rule.allow_unprivileged) {
                    std::cerr << "[DBusPolicy] SECURITY VIOLATION: Unprivileged UID " << sender_uid 
                              << " attempted to invoke restricted method " << interface << "." << method << "\n";
                    return DBusAccessDecision::DENY;
                }
            }
        }

        // Default allow for standard unprivileged app methods (e.g., FileIndexer search)
        return DBusAccessDecision::ALLOW;
    }
};

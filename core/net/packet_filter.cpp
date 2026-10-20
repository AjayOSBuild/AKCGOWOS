#include "packet_filter.h"
#include <iomanip>

PacketFilterManager::PacketFilterManager() {
    std::cout << "[NetFilter] Initializing Packet Filter & Firewall Engine (nftables wrapper)...\n";
    // Setup default loopback policy
    AddRule({FilterChain::INPUT, "any", "127.0.0.1", "127.0.0.1", -1, FilterAction::ACCEPT});
}

std::string PacketFilterManager::ActionToString(FilterAction action) const {
    switch (action) {
        case FilterAction::ACCEPT: return "ACCEPT";
        case FilterAction::DROP: return "DROP";
        case FilterAction::REJECT: return "REJECT";
        case FilterAction::MASQUERADE: return "MASQUERADE (NAT)";
        default: return "UNKNOWN";
    }
}

std::string PacketFilterManager::ChainToString(FilterChain chain) const {
    switch (chain) {
        case FilterChain::INPUT: return "INPUT";
        case FilterChain::OUTPUT: return "OUTPUT";
        case FilterChain::FORWARD: return "FORWARD";
        default: return "UNKNOWN";
    }
}

bool PacketFilterManager::MatchesRule(const NetworkPacket& packet, const FilterRule& rule) const {
    if (rule.protocol != "any" && rule.protocol != packet.protocol) return false;
    if (rule.source_ip != "any" && rule.source_ip != packet.source_ip) return false;
    if (rule.dest_ip != "any" && rule.dest_ip != packet.dest_ip) return false;
    if (rule.dest_port != -1 && rule.dest_port != packet.dest_port) return false;
    return true;
}

void PacketFilterManager::AddRule(const FilterRule& rule) {
    rules.push_back(rule);
    std::cout << "[NetFilter] Added rule to " << ChainToString(rule.chain) 
              << " chain (Action: " << ActionToString(rule.action) << ").\n";
}

void PacketFilterManager::FlushRules() {
    rules.clear();
    std::cout << "[NetFilter] Flushed all firewall rules.\n";
}

FilterAction PacketFilterManager::EvaluatePacket(FilterChain chain, const NetworkPacket& packet) const {
    std::cout << "[NetFilter] Evaluating packet [" << packet.protocol << "] " 
              << packet.source_ip << " -> " << packet.dest_ip << ":" << packet.dest_port 
              << " on chain " << ChainToString(chain) << "...\n";

    // Standard top-down rule evaluation
    for (const auto& rule : rules) {
        if (rule.chain == chain && MatchesRule(packet, rule)) {
            std::cout << "  -> Rule match found! Applying action: " << ActionToString(rule.action) << "\n";
            return rule.action;
        }
    }
    
    // Default policy (DROP if no match)
    std::cout << "  -> No rules matched. Applying default policy: DROP\n";
    return FilterAction::DROP;
}

void PacketFilterManager::DisplayRuleset() const {
    std::cout << "\n--- Active Firewall Ruleset ---\n";
    for (const auto& rule : rules) {
        std::cout << "Chain " << std::setw(7) << std::left << ChainToString(rule.chain) 
                  << " | Proto: " << std::setw(4) << rule.protocol 
                  << " | Src: " << std::setw(15) << rule.source_ip 
                  << " | Dst: " << std::setw(15) << rule.dest_ip 
                  << " | DPort: " << std::setw(5) << (rule.dest_port == -1 ? "any" : std::to_string(rule.dest_port))
                  << " | Action: " << ActionToString(rule.action) << "\n";
    }
    std::cout << "-------------------------------\n";
}

int main() {
    std::cout << "=== OS Project Packet Filter & Firewall Utility ===\n";
    PacketFilterManager firewall;

    std::cout << "\n--- Configuring Security Policies ---\n";
    // Allow incoming HTTP traffic
    firewall.AddRule({FilterChain::INPUT, "tcp", "any", "any", 80, FilterAction::ACCEPT});
    // Allow outgoing traffic
    firewall.AddRule({FilterChain::OUTPUT, "any", "any", "any", -1, FilterAction::ACCEPT});
    // Setup NAT masquerade for containers
    firewall.AddRule({FilterChain::FORWARD, "any", "10.0.0.0/24", "any", -1, FilterAction::MASQUERADE});
    // Drop specific malicious IP
    firewall.AddRule({FilterChain::INPUT, "tcp", "198.51.100.22", "any", 22, FilterAction::DROP});

    firewall.DisplayRuleset();

    std::cout << "\n--- Simulating Network Traffic ---\n";
    
    // Simulating valid web traffic
    NetworkPacket p1 = {"tcp", "203.0.113.5", "192.168.1.100", 80};
    firewall.EvaluatePacket(FilterChain::INPUT, p1);

    // Simulating unauthorized SSH access
    NetworkPacket p2 = {"tcp", "198.51.100.22", "192.168.1.100", 22};
    firewall.EvaluatePacket(FilterChain::INPUT, p2);

    std::cout << "\n[+] Packet Filter & Firewall testing completed successfully.\n";
    return 0;
}

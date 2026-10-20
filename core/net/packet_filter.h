#pragma once
#include <iostream>
#include <string>
#include <vector>

enum class FilterChain {
    INPUT,
    OUTPUT,
    FORWARD
};

enum class FilterAction {
    ACCEPT,
    DROP,
    REJECT,
    MASQUERADE // NAT/SNAT
};

struct FilterRule {
    FilterChain chain;
    std::string protocol;    // "tcp", "udp", "icmp", "any"
    std::string source_ip;   // IP or "any"
    std::string dest_ip;     // IP or "any"
    int dest_port;           // Port number or -1 for "any"
    FilterAction action;
};

struct NetworkPacket {
    std::string protocol;
    std::string source_ip;
    std::string dest_ip;
    int dest_port;
};

class PacketFilterManager {
private:
    std::vector<FilterRule> rules;

    bool MatchesRule(const NetworkPacket& packet, const FilterRule& rule) const;
    std::string ActionToString(FilterAction action) const;
    std::string ChainToString(FilterChain chain) const;

public:
    PacketFilterManager();

    // Appends a new rule to the firewall chains
    void AddRule(const FilterRule& rule);

    // Flushes all active firewall rules
    void FlushRules();

    // Evaluates an incoming or outgoing packet against the ruleset
    FilterAction EvaluatePacket(FilterChain chain, const NetworkPacket& packet) const;

    // Displays the active firewall rules
    void DisplayRuleset() const;
};

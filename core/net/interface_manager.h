#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

enum class InterfaceType {
    PHYSICAL,
    VIRTUAL,
    LOOPBACK
};

struct NetworkInterface {
    std::string name;
    std::string mac_address;
    std::string ipv4_address;
    InterfaceType type;
    bool is_up;
};

class InterfaceManager {
private:
    std::unordered_map<std::string, NetworkInterface> interfaces;

    // Helper to generate a random mock MAC address
    std::string GenerateMockMAC() const;

public:
    InterfaceManager();

    // Simulates reading Netlink sockets to discover system interfaces
    void EnumerateInterfaces();

    // Changes the administrative state of a network link
    bool SetInterfaceState(const std::string& iface_name, bool state_up);

    // Provisions an IPv4 address (simulating DHCP or static assignment)
    bool AssignIPv4(const std::string& iface_name, const std::string& ip_address);

    // Displays the current routing and interface table
    void DisplayInterfaces() const;
};

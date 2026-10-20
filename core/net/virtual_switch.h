#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct Bridge {
    std::string name;
    bool is_up;
    std::vector<std::string> attached_ports;
};

class VirtualSwitch {
private:
    std::unordered_map<std::string, Bridge> bridges;

public:
    VirtualSwitch();

    // Creates a new software bridge interface
    bool CreateBridge(const std::string& bridge_name);

    // Attaches a network interface (e.g., veth pair) to the bridge
    bool AttachPort(const std::string& bridge_name, const std::string& interface_name);

    // Detaches a network interface from the bridge
    bool DetachPort(const std::string& bridge_name, const std::string& interface_name);

    // Simulates bringing the bridge interface up
    bool SetBridgeState(const std::string& bridge_name, bool state_up);

    // Displays current vSwitch topology
    void DisplayTopology() const;
};

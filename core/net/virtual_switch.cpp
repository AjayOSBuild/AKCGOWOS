#include "virtual_switch.h"
#include <algorithm>

VirtualSwitch::VirtualSwitch() {
    std::cout << "[vSwitch] Initializing Virtual Switch (Software Bridge) Manager...\n";
}

bool VirtualSwitch::CreateBridge(const std::string& bridge_name) {
    if (bridges.find(bridge_name) != bridges.end()) {
        std::cerr << "[!] Bridge '" << bridge_name << "' already exists.\n";
        return false;
    }
    bridges[bridge_name] = {bridge_name, false, {}};
    std::cout << "[vSwitch] Created virtual bridge interface: " << bridge_name << "\n";
    return true;
}

bool VirtualSwitch::AttachPort(const std::string& bridge_name, const std::string& interface_name) {
    auto it = bridges.find(bridge_name);
    if (it == bridges.end()) {
        std::cerr << "[!] Bridge '" << bridge_name << "' not found.\n";
        return false;
    }
    
    auto& ports = it->second.attached_ports;
    if (std::find(ports.begin(), ports.end(), interface_name) != ports.end()) {
        std::cerr << "[!] Port '" << interface_name << "' is already attached to '" << bridge_name << "'.\n";
        return false;
    }
    
    ports.push_back(interface_name);
    std::cout << "[vSwitch] Attached interface '" << interface_name << "' to bridge '" << bridge_name << "'.\n";
    return true;
}

bool VirtualSwitch::DetachPort(const std::string& bridge_name, const std::string& interface_name) {
    auto it = bridges.find(bridge_name);
    if (it == bridges.end()) return false;
    
    auto& ports = it->second.attached_ports;
    auto port_it = std::find(ports.begin(), ports.end(), interface_name);
    if (port_it != ports.end()) {
        ports.erase(port_it);
        std::cout << "[vSwitch] Detached interface '" << interface_name << "' from bridge '" << bridge_name << "'.\n";
        return true;
    }
    return false;
}

bool VirtualSwitch::SetBridgeState(const std::string& bridge_name, bool state_up) {
    auto it = bridges.find(bridge_name);
    if (it == bridges.end()) return false;
    
    it->second.is_up = state_up;
    std::cout << "[vSwitch] Bridge '" << bridge_name << "' state changed to: " << (state_up ? "UP" : "DOWN") << "\n";
    return true;
}

void VirtualSwitch::DisplayTopology() const {
    std::cout << "\n--- vSwitch Topology ---\n";
    for (const auto& pair : bridges) {
        const auto& bridge = pair.second;
        std::cout << "Bridge: " << bridge.name << " [State: " << (bridge.is_up ? "UP" : "DOWN") << "]\n";
        if (bridge.attached_ports.empty()) {
            std::cout << "  (No ports attached)\n";
        } else {
            for (const auto& port : bridge.attached_ports) {
                std::cout << "  |-- Port: " << port << "\n";
            }
        }
    }
    std::cout << "------------------------\n";
}

int main() {
    std::cout << "=== OS Project Virtual Switch Utility ===\n";
    VirtualSwitch vswitch;

    std::cout << "\n--- Configuring Container Network Bridge ---\n";
    vswitch.CreateBridge("nexus-br0");
    vswitch.SetBridgeState("nexus-br0", true);
    
    // Simulate attaching veth pairs for two isolated containers
    vswitch.AttachPort("nexus-br0", "veth_c1");
    vswitch.AttachPort("nexus-br0", "veth_c2");
    
    vswitch.DisplayTopology();

    std::cout << "\n[+] Virtual Switch testing completed successfully.\n";
    return 0;
}

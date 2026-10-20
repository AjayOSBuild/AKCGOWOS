#include "interface_manager.h"
#include <iomanip>
#include <sstream>
#include <cstdlib>

InterfaceManager::InterfaceManager() {
    std::cout << "[NetManager] Initializing Network Interface Manager...\n";
}

std::string InterfaceManager::GenerateMockMAC() const {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 6; ++i) {
        ss << std::setw(2) << (rand() % 256);
        if (i < 5) ss << ":";
    }
    return ss.str();
}

void InterfaceManager::EnumerateInterfaces() {
    std::cout << "[NetManager] Enumerating kernel network interfaces via Netlink...\n";
    
    // Mock interface discovery
    interfaces["lo"] = {"lo", "00:00:00:00:00:00", "127.0.0.1", InterfaceType::LOOPBACK, true};
    interfaces["eth0"] = {"eth0", GenerateMockMAC(), "", InterfaceType::PHYSICAL, false};
    interfaces["veth0"] = {"veth0", GenerateMockMAC(), "", InterfaceType::VIRTUAL, false};
    
    std::cout << "  -> Discovered " << interfaces.size() << " interfaces.\n";
}

bool InterfaceManager::SetInterfaceState(const std::string& iface_name, bool state_up) {
    auto it = interfaces.find(iface_name);
    if (it == interfaces.end()) {
        std::cerr << "[!] Interface '" << iface_name << "' not found.\n";
        return false;
    }

    it->second.is_up = state_up;
    std::cout << "[NetManager] Interface '" << iface_name << "' state changed to: " 
              << (state_up ? "UP" : "DOWN") << "\n";
    return true;
}

bool InterfaceManager::AssignIPv4(const std::string& iface_name, const std::string& ip_address) {
    auto it = interfaces.find(iface_name);
    if (it == interfaces.end()) {
        std::cerr << "[!] Interface '" << iface_name << "' not found.\n";
        return false;
    }

    if (!it->second.is_up) {
        std::cerr << "[!] Cannot assign IP. Interface '" << iface_name << "' is DOWN.\n";
        return false;
    }

    it->second.ipv4_address = ip_address;
    std::cout << "[NetManager] Assigned IPv4 address " << ip_address << " to '" << iface_name << "'\n";
    return true;
}

void InterfaceManager::DisplayInterfaces() const {
    std::cout << "\n--- Network Interfaces ---\n";
    for (const auto& pair : interfaces) {
        const auto& iface = pair.second;
        std::string type_str = (iface.type == InterfaceType::PHYSICAL) ? "Physical" : 
                               (iface.type == InterfaceType::VIRTUAL) ? "Virtual" : "Loopback";
        
        std::cout << iface.name << "  Link encap: " << type_str << "  HWaddr: " << iface.mac_address << "\n"
                  << "        inet addr: " << (iface.ipv4_address.empty() ? "UNASSIGNED" : iface.ipv4_address) << "\n"
                  << "        UP: " << (iface.is_up ? "YES" : "NO") << "\n\n";
    }
    std::cout << "--------------------------\n";
}

int main() {
    std::cout << "=== OS Project Network Interface Utility ===\n";
    InterfaceManager net_mgr;

    net_mgr.EnumerateInterfaces();
    
    std::cout << "\n--- Provisioning Physical Link ---\n";
    net_mgr.SetInterfaceState("eth0", true);
    net_mgr.AssignIPv4("eth0", "192.168.1.100");

    std::cout << "\n--- Provisioning Virtual Link (Container) ---\n";
    net_mgr.SetInterfaceState("veth0", true);
    net_mgr.AssignIPv4("veth0", "10.0.0.5");

    net_mgr.DisplayInterfaces();

    std::cout << "[+] Network Interface Manager testing completed successfully.\n";
    return 0;
}

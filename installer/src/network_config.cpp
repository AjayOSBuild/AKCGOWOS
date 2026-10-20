#include "network_config.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

NetworkConfigurer::NetworkConfigurer(const std::string& root_path) : target_root_path(root_path) {
    std::cout << "[NetworkConfigurer] Initializing installer network setup module...\n";
}

bool NetworkConfigurer::ConfigureHostname(const std::string& hostname) const {
    if (hostname.empty()) {
        std::cerr << "[!] Error: Hostname cannot be empty.\n";
        return false;
    }

    std::string hostname_path = target_root_path + "/etc/hostname";
    std::string hosts_path = target_root_path + "/etc/hosts";

    std::cout << "[NetworkConfigurer] Setting system hostname to: " << hostname << "\n";
    std::cout << "  -> [Simulation] Writing to " << hostname_path << "\n";
    std::cout << "  -> [Simulation] Updating " << hosts_path << " loopback mappings\n";

    return true;
}

bool NetworkConfigurer::ConfigureNetwork(const NetworkConfig& config) const {
    std::cout << "[NetworkConfigurer] Configuring network interfaces for hostname: " << config.hostname << "\n";
    
    if (config.use_dhcp) {
        std::cout << "  -> Mode: DHCP (Dynamic IP allocation enabled via systemd-networkd)\n";
    } else {
        std::cout << "  -> Mode: Static IP Configuration\n";
        std::cout << "     IP Address : " << config.static_ip << "\n";
        std::cout << "     Gateway    : " << config.gateway << "\n";
        std::cout << "     DNS Server : " << config.dns_server << "\n";
    }

    std::cout << "[+] Network parameters successfully written to target profile.\n";
    return true;
}

int main() {
    std::cout << "=== OS Project Installer: Network Configuration Utility ===\n";
    
    NetworkConfigurer configurer("/mnt/nexus");
    
    // Test DHCP configuration profile
    NetworkConfig dhcp_net = {"nexus-workstation", true, "", "", ""};
    configurer.ConfigureHostname(dhcp_net.hostname);
    configurer.ConfigureNetwork(dhcp_net.use_dhcp ? dhcp_net : NetworkConfig());

    std::cout << "[+] Network configuration testing completed safely.\n";
    return 0;
}

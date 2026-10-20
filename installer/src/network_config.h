#pragma once
#include <string>

struct NetworkConfig {
    std::string hostname;
    bool use_dhcp;
    std::string static_ip;
    std::string gateway;
    std::string dns_server;
};

class NetworkConfigurer {
private:
    std::string target_root_path;

public:
    explicit NetworkConfigurer(const std::string& root_path = "/mnt/nexus");

    // Sets the system hostname on the target filesystem
    bool ConfigureHostname(const std::string& hostname) const;

    // Writes network interface configuration (DHCP or Static)
    bool ConfigureNetwork(const NetworkConfig& config) const;
};

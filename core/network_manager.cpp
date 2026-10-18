#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;

void check_network_interfaces() {
    std::string net_path = "/sys/class/net";
    std::cout << "[NetworkManager] Scanning for network interfaces...\n";
    if (fs::exists(net_path)) {
        for (const auto& entry : fs::directory_iterator(net_path)) {
            std::string iface = entry.path().filename().string();
            // Ignore the loopback interface for now
            if (iface != "lo") {
                std::cout << "  - Detected interface: " << iface << "\n";
            }
        }
    } else {
        std::cout << "  [!] /sys/class/net not found. Is sysfs mounted?\n";
    }
}

int main() {
    std::cout << "=== OS Project Network Manager (Ring 3) ===\n";
    
    check_network_interfaces();

    std::cout << "[+] Network Manager active. Ready to orchestrate DHCP and routing...\n";

    // In future phases, this daemon will listen on D-Bus for connection 
    // requests from the desktop UI, execute DHCP client routines, and manage DNS.
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}

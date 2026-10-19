#include <iostream>
#include "device_manager.h"

int main() {
    std::cout << "=== OS Project Advanced System Device Manager Utility ===\n";
    SystemDeviceManager dev_mgr;
    
    // Re-scan hardware tree to verify sysfs parsing
    dev_mgr.EnumerateHardware();
    
    std::cout << "[+] Advanced Device Manager utility initialized successfully.\n";
    return 0;
}

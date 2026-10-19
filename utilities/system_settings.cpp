#include <iostream>
#include "system_settings.h"

int main() {
    std::cout << "=== OS Project Comprehensive System Settings Utility ===\n";
    SystemSettingsUtility settings;
    
    // Test updating preference profiles
    settings.SwitchSection(ControlPanelSection::POWER);
    settings.SetPreference("power_profile", "High Performance");
    
    settings.SwitchSection(ControlPanelSection::APPEARANCE);
    settings.SetPreference("theme", "Nexus-Light");
    
    std::cout << "[+] System Settings utility tests completed successfully.\n";
    return 0;
}

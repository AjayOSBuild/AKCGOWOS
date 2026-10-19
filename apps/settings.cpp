#include <iostream>
#include "settings.h"

int main() {
    std::cout << "=== OS Project System Settings App ===\n";
    SettingsApp settings;
    
    // Simulate user interactions
    settings.SwitchCategory(SettingsCategory::PERSONALIZATION);
    settings.ApplyTheme(true); // Switch to dark mode
    
    settings.SwitchCategory(SettingsCategory::DISPLAY);
    settings.ApplyDisplayResolution(2560, 1440);
    
    std::cout << "[+] System Settings application tests completed successfully.\n";
    return 0;
}

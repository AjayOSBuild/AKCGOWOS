#include <iostream>
#include <chrono>
#include <thread>
#include "package_gui.h"

int main() {
    std::cout << "=== OS Project Package Manager GUI ===\n";
    PackageManagerGUI app_store;
    
    // Simulate user selecting an app and initiating installation
    app_store.HandleKeyboardEvent(80, true); // Move down to select an app
    app_store.HandleKeyboardEvent(10, true); // Press Enter to view details
    app_store.HandleKeyboardEvent(10, true); // Press Enter to install
    
    // UI Event Loop (Simulated)
    int ticks = 0;
    while (ticks < 5) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ticks++;
    }
    
    std::cout << "[+] Package GUI application loop exited cleanly.\n";
    return 0;
}

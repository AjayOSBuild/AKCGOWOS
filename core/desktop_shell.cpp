#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "taskbar_service.h"
#include "system_tray.h"
#include "start_menu.h"

int main() {
    std::cout << "=== OS Project Desktop Shell (Ring 3) ===\n";
    std::cout << "[*] Initializing desktop environment UI client...\n";
    
    std::cout << "[DesktopShell] Connecting to Display Server IPC (/run/os/display.sock)...\n";
    std::cout << "[DesktopShell] Requesting root background surface (1024x768)...\n";

    TaskbarService taskbar;
    SystemTray tray;
    StartMenu start_menu;

    std::cout << "[+] Desktop Shell active. Ready to render workspace.\n";

    // Simulate opening the start menu after 2 seconds
    std::thread mock_input_thread([&start_menu]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "\n[Input Simulation] User clicked Start button...\n";
        start_menu.Toggle();
    });
    mock_input_thread.detach();

    while (true) {
        // Render pass (Painter's algorithm: background -> taskbar -> overlays)
        taskbar.Render();
        tray.Render();
        start_menu.Render();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Match compositor 60fps
    }

    return 0;
}

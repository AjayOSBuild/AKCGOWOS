#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "taskbar_service.h"
#include "system_tray.h"
#include "start_menu.h"
#include "notification_center.h"
#include "window_manager_ui.h"

int main() {
    std::cout << "=== OS Project Desktop Shell (Ring 3) ===\n";
    std::cout << "[*] Initializing desktop environment UI client...\n";
    
    std::cout << "[DesktopShell] Connecting to Display Server IPC (/run/os/display.sock)...\n";
    std::cout << "[DesktopShell] Requesting root background surface (1024x768)...\n";

    TaskbarService taskbar;
    SystemTray tray;
    StartMenu start_menu;
    NotificationCenter notif_center;
    WindowManagerUI wm_ui;

    // Simulate an application window registering for decorations
    wm_ui.RegisterWindow(101, "File Explorer", 150, 150, 800, 600);

    std::cout << "[+] Desktop Shell active. Ready to render workspace.\n";

    // Simulate input events interacting with window decorations
    std::thread mock_events_thread([&wm_ui]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "\n[Input Simulation] User clicked on window title bar...\n";
        wm_ui.HandleClick(500, 160); // Drag zone
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n[Input Simulation] User clicked window close button...\n";
        wm_ui.HandleClick(940, 160); // Close button zone
    });
    mock_events_thread.detach();

    while (true) {
        // Render pass (Painter's algorithm)
        // 1. Background
        // 2. Application surfaces (handled by compositor)
        // 3. Window decorations
        wm_ui.Render();
        // 4. Desktop overlays (Taskbar, menus)
        taskbar.Render();
        tray.Render();
        start_menu.Render();
        notif_center.Render();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Match compositor 60fps
    }

    return 0;
}

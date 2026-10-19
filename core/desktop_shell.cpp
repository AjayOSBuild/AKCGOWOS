#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "taskbar_service.h"
#include "system_tray.h"
#include "start_menu.h"
#include "notification_center.h"

int main() {
    std::cout << "=== OS Project Desktop Shell (Ring 3) ===\n";
    std::cout << "[*] Initializing desktop environment UI client...\n";
    
    std::cout << "[DesktopShell] Connecting to Display Server IPC (/run/os/display.sock)...\n";
    std::cout << "[DesktopShell] Requesting root background surface (1024x768)...\n";

    TaskbarService taskbar;
    SystemTray tray;
    StartMenu start_menu;
    NotificationCenter notif_center;

    std::cout << "[+] Desktop Shell active. Ready to render workspace.\n";

    // Simulate input and incoming IPC notifications
    std::thread mock_events_thread([&start_menu, &notif_center]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        start_menu.Toggle();
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n[IPC Simulation] Incoming D-Bus alert from NetworkManager...\n";
        notif_center.PushNotification("NetworkManager", "Wi-Fi Connected", "Successfully joined network.");
    });
    mock_events_thread.detach();

    while (true) {
        // Render pass (Painter's algorithm: background -> taskbar -> overlays)
        taskbar.Render();
        tray.Render();
        start_menu.Render();
        notif_center.Render();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Match compositor 60fps
    }

    return 0;
}

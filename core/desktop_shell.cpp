#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "taskbar_service.h"

int main() {
    std::cout << "=== OS Project Desktop Shell (Ring 3) ===\n";
    std::cout << "[*] Initializing desktop environment UI client...\n";
    
    std::cout << "[DesktopShell] Connecting to Display Server IPC (/run/os/display.sock)...\n";
    std::cout << "[DesktopShell] Requesting root background surface (1024x768)...\n";

    TaskbarService taskbar;

    std::cout << "[+] Desktop Shell active. Ready to render workspace.\n";

    while (true) {
        // Render pass
        // desktop_background.Render();
        taskbar.Render();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Match compositor 60fps
    }

    return 0;
}

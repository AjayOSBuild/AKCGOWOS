#include <iostream>
#include <chrono>
#include <thread>
#include "compositor.h"

int main() {
    std::cout << "=== OS Project Graphics Compositor & Display Server (Ring 3) ===\n";
    std::cout << "[*] Opening DRM/KMS graphics device (/dev/dri/card0)...\n";

    CompositorEngine compositor;

    // Create background and test windows
    int bg_id = compositor.CreateWindow("Nexus Desktop Shell", 0, 0, 1024, 768);
    int app1_id = compositor.CreateWindow("Terminal", 100, 100, 800, 600);
    int app2_id = compositor.CreateWindow("File Explorer", 150, 150, 800, 600);

    // Simulate window management actions
    if (auto win = compositor.GetWindow(app1_id)) {
        win->Move(120, 120);
    }
    
    // Bring Terminal back to the front
    compositor.SetFocus(app1_id);

    std::cout << "[+] Compositor active. Entering 60fps render loop with z-order management...\n";

    while (true) {
        compositor.RenderFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    return 0;
}

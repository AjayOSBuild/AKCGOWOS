#include <iostream>
#include <chrono>
#include <thread>
#include "compositor.h"

int main() {
    std::cout << "=== OS Project Graphics Compositor & Display Server (Ring 3) ===\n";
    std::cout << "[*] Opening DRM/KMS graphics device (/dev/dri/card0)...\n";
    std::cout << "[*] Opening input devices (/dev/input/event*)...\n";

    CompositorEngine compositor;

    int bg_id = compositor.CreateWindow("Nexus Desktop Shell", 0, 0, 1024, 768);
    int app1_id = compositor.CreateWindow("Terminal", 100, 100, 800, 600);
    
    compositor.SetFocus(app1_id);

    std::cout << "\n[+] Compositor active. Entering 60fps render & input polling loop...\n";

    while (true) {
        compositor.RenderFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    return 0;
}

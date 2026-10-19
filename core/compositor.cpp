#include <iostream>
#include <chrono>
#include <thread>
#include "compositor.h"

int main() {
    std::cout << "=== OS Project Graphics Compositor & Display Server (Ring 3) ===\n";
    std::cout << "[*] Opening DRM/KMS graphics device (/dev/dri/card0)...\n";

    CompositorEngine compositor;

    // Create a mock desktop background or taskbar shell surface
    compositor.CreateWindow("Nexus Desktop Shell", 0, 0, 1024, 768);

    std::cout << "[+] Compositor active. Entering 60fps render loop...\n";

    while (true) {
        compositor.RenderFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    return 0;
}

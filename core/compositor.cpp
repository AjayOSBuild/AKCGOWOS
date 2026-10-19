#include <iostream>
#include <chrono>
#include <thread>
#include "compositor.h"

int main() {
    std::cout << "=== OS Project Graphics Compositor & Display Server (Ring 3) ===\n";
    std::cout << "[*] Opening DRM/KMS graphics device (/dev/dri/card0)...\n";
    std::cout << "[*] Opening input devices (/dev/input/event*)...\n";

    CompositorEngine compositor;
    DisplayServerIPC ipc_server(compositor);

    int bg_id = compositor.CreateWindow("Nexus Desktop Shell", 0, 0, 1024, 768);
    
    // Simulate an external application connecting over IPC and creating a window
    std::cout << "\n[DisplayServerIPC] Simulating incoming client connection...\n";
    int app1_id = compositor.CreateWindow("ExternalApp (via IPC)", 100, 100, 800, 600);
    compositor.AttachBuffer(app1_id);
    compositor.SetFocus(app1_id);

    std::cout << "\n[+] Compositor active. Entering 60fps render & IPC polling loop...\n";

    while (true) {
        ipc_server.PollClientRequests();
        compositor.RenderFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    return 0;
}

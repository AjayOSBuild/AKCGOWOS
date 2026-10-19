#include <iostream>
#include <chrono>
#include <thread>
#include "compositor.h"

int main() {
    std::cout << "=== OS Project Graphics Compositor & Display Server (Ring 3) ===\n";
    std::cout << "[*] Opening DRM/KMS graphics device (/dev/dri/card0)...\n";
    std::cout << "[*] Opening input devices (/dev/input/event*)...\n";

    CompositorEngine compositor;

    // Create background and test windows
    int bg_id = compositor.CreateWindow("Nexus Desktop Shell", 0, 0, 1024, 768);
    int app1_id = compositor.CreateWindow("Terminal", 100, 100, 800, 600);
    int app2_id = compositor.CreateWindow("File Explorer", 150, 150, 800, 600);

    // Terminal regains focus
    compositor.SetFocus(app1_id);

    // Simulate input events polled from /dev/input
    std::cout << "\n[Input Subsystem] Simulating incoming evdev events...\n";
    
    // Simulate a keystroke (e.g., 'ls' typed into Terminal)
    compositor.DispatchInputEvent({InputType::KEYBOARD, 38, 0, 0, true});  // 'L' key down
    compositor.DispatchInputEvent({InputType::KEYBOARD, 38, 0, 0, false}); // 'L' key up
    
    // Simulate a mouse click relative to screen coordinates
    compositor.DispatchInputEvent({InputType::POINTER_CLICK, 0, 200, 200, true});

    std::cout << "\n[+] Compositor active. Entering 60fps render & input polling loop...\n";

    while (true) {
        compositor.RenderFrame();
        // In full execution, non-blocking epoll on input FDs occurs here
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    return 0;
}

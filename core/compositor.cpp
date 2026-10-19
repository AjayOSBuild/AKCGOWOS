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
    
    // Position the Terminal
    if (auto win = compositor.GetWindow(app1_id)) {
        win->Move(100, 100);
    }
    compositor.SetFocus(app1_id);

    // Simulate input events polled from /dev/input
    std::cout << "\n[Input Subsystem] Simulating incoming evdev events...\n";
    
    // Simulate pointer moving across the screen
    compositor.DispatchInputEvent({InputType::POINTER_MOVE, 0, 50, 50, false});   // Outside Terminal
    compositor.DispatchInputEvent({InputType::POINTER_MOVE, 0, 150, 150, false}); // Inside Terminal
    
    // Simulate a mouse click while inside bounds
    compositor.DispatchInputEvent({InputType::POINTER_CLICK, 0, 150, 150, true});

    std::cout << "\n[+] Compositor active. Entering 60fps render & input polling loop...\n";

    while (true) {
        compositor.RenderFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    return 0;
}

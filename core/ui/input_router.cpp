#include "input_router.h"

InputRouter::InputRouter() {
    std::cout << "[InputRouter] Initializing Hardware Input Event Multiplexer (evdev)...\n";
}

void InputRouter::SetFocusedSurface(uint32_t surface_id) {
    if (focused_surface_id != surface_id) {
        std::cout << "[InputRouter] Focus shifted to Surface ID " << surface_id << ".\n";
        focused_surface_id = surface_id;
    }
}

uint32_t InputRouter::HitTest(int x, int y) const {
    // In production, this queries the WindowCompositor for the top-most visible 
    // bounding box containing (x,y). For simulation, we return a mock surface.
    std::cout << "  -> [HitTest] Resolving top-most window at (" << x << "," << y << ")...\n";
    return 2; // Simulate hitting Surface ID 2
}

void InputRouter::DispatchEvent(uint32_t surface_id, const InputEvent& ev) const {
    std::cout << "  -> [IPC] Dispatching event to Client owning Surface ID " << surface_id << "\n";
}

void InputRouter::ProcessRawEvent(const InputEvent& ev) {
    std::cout << "[InputRouter] Processing raw hardware event...\n";

    if (ev.type == InputEventType::MOUSE_CLICK_DOWN) {
        // Handle window focus on click
        uint32_t hit_surface = HitTest(ev.code_or_x, ev.value_or_y);
        SetFocusedSurface(hit_surface);
        DispatchEvent(hit_surface, ev);
    } 
    else if (ev.type == InputEventType::MOUSE_MOVE) {
        // Mouse moves typically dispatch to whoever is under the cursor
        uint32_t hit_surface = HitTest(ev.code_or_x, ev.value_or_y);
        DispatchEvent(hit_surface, ev);
    } 
    else if (ev.type == InputEventType::KEYBOARD_PRESS || ev.type == InputEventType::KEYBOARD_RELEASE) {
        // Keyboard events always go to the focused surface
        if (focused_surface_id != 0) {
            std::cout << "  -> Keyboard event (Code: " << ev.code_or_x << "). Routing to focused surface.\n";
            DispatchEvent(focused_surface_id, ev);
        } else {
            std::cout << "  -> [!] Keyboard event dropped. No surface is focused.\n";
        }
    }
}

int main() {
    std::cout << "=== OS Project Input Event Router Utility ===\n";
    InputRouter router;

    std::cout << "\n--- Simulating User Mouse Click ---\n";
    // User clicks at X:450, Y:350
    InputEvent click = {InputEventType::MOUSE_CLICK_DOWN, 450, 350, 1000120};
    router.ProcessRawEvent(click);

    std::cout << "\n--- Simulating User Keystroke ---\n";
    // User presses keycode 30 (e.g., 'A')
    InputEvent key_press = {InputEventType::KEYBOARD_PRESS, 30, 0, 1000150};
    router.ProcessRawEvent(key_press);

    std::cout << "\n[+] Input Event Router testing completed successfully.\n";
    return 0;
}

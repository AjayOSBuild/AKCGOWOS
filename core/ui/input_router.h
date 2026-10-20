#pragma once
#include <iostream>
#include <string>
#include <vector>

enum class InputEventType {
    KEYBOARD_PRESS,
    KEYBOARD_RELEASE,
    MOUSE_MOVE,
    MOUSE_CLICK_DOWN,
    MOUSE_CLICK_UP
};

struct InputEvent {
    InputEventType type;
    int code_or_x;     // Keycode for keyboard, X coord for mouse
    int value_or_y;    // Modifiers for keyboard, Y coord for mouse
    uint32_t timestamp;
};

class InputRouter {
private:
    uint32_t focused_surface_id = 0; // 0 means no focus or root desktop

    // Simulates serializing and sending the event over IPC to the client
    void DispatchEvent(uint32_t surface_id, const InputEvent& ev) const;

public:
    InputRouter();

    // Sets which application window currently receives keyboard/mouse events
    void SetFocusedSurface(uint32_t surface_id);

    // Processes raw hardware input (e.g., from /dev/input/event*)
    void ProcessRawEvent(const InputEvent& ev);
    
    // Calculates which surface is under the mouse coordinates (hit testing mock)
    uint32_t HitTest(int x, int y) const;
};

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

enum class InputType { KEYBOARD, POINTER_MOVE, POINTER_CLICK };

struct InputEvent {
    InputType type;
    int key_code; // For KEYBOARD
    int x, y;     // For POINTER
    bool pressed; // Key/Button state
};

struct HardwareCursor {
    int x = 0, y = 0;
    bool visible = true;

    void Move(int new_x, int new_y) {
        x = new_x;
        y = new_y;
        // Native DRM implementation calls drmModeMoveCursor() here
        std::cout << "[Compositor:DRM] Hardware cursor updated to absolute (" << x << "," << y << ")\n";
    }
};

struct WindowSurface {
    int surface_id;
    std::string title;
    int x, y, width, height;
    bool visible = true;
    int z_index = 0;
    bool is_focused = false;

    void Move(int new_x, int new_y) {
        x = new_x;
        y = new_y;
        std::cout << "[Compositor] Window #" << surface_id << " moved to (" << x << "," << y << ")\n";
    }

    void Resize(int new_width, int new_height) {
        width = new_width;
        height = new_height;
        std::cout << "[Compositor] Window #" << surface_id << " resized to " << width << "x" << height << "\n";
    }

    void HandleInput(const InputEvent& event) {
        if (event.type == InputType::KEYBOARD) {
            std::cout << "[Window #" << surface_id << "] Received KEY " << (event.pressed ? "DOWN" : "UP") 
                      << " (Code: " << event.key_code << ")\n";
        } else if (event.type == InputType::POINTER_CLICK) {
            std::cout << "[Window #" << surface_id << "] Received CLICK " << (event.pressed ? "DOWN" : "UP") 
                      << " at relative (" << event.x - x << "," << event.y - y << ")\n";
        } else if (event.type == InputType::POINTER_MOVE) {
            std::cout << "[Window #" << surface_id << "] Received HOVER at relative (" << event.x - x << "," << event.y - y << ")\n";
        }
    }
};

class CompositorEngine {
private:
    std::vector<WindowSurface> surfaces;
    int next_id = 1;
    HardwareCursor cursor;

    void SortSurfaces() {
        std::sort(surfaces.begin(), surfaces.end(), [](const WindowSurface& a, const WindowSurface& b) {
            return a.z_index < b.z_index;
        });
    }

public:
    CompositorEngine() {
        std::cout << "[Compositor] Graphics engine initialized with DRM Hardware Cursor support.\n";
    }

    int CreateWindow(const std::string& title, int x, int y, int width, int height) {
        WindowSurface surf{next_id++, title, x, y, width, height, true, 0, false};
        surfaces.push_back(surf);
        std::cout << "[Compositor] Created window surface #" << surf.surface_id << ": " << title << "\n";
        SetFocus(surf.surface_id);
        return surf.surface_id;
    }

    void SetFocus(int surface_id) {
        int highest_z = 0;
        for (auto& surf : surfaces) {
            if (surf.z_index > highest_z) {
                highest_z = surf.z_index;
            }
            surf.is_focused = false;
        }
        
        for (auto& surf : surfaces) {
            if (surf.surface_id == surface_id) {
                surf.is_focused = true;
                surf.z_index = highest_z + 1;
                std::cout << "[Compositor] Window #" << surface_id << " focused (z_index=" << surf.z_index << ")\n";
            }
        }
        SortSurfaces();
    }

    WindowSurface* GetWindow(int surface_id) {
        for (auto& surf : surfaces) {
            if (surf.surface_id == surface_id) return &surf;
        }
        return nullptr;
    }

    void DispatchInputEvent(const InputEvent& event) {
        // 1. Update standalone hardware cursor plane
        if (event.type == InputType::POINTER_MOVE) {
            cursor.Move(event.x, event.y);
        }

        // 2. Route the event to the focused window with bounding box checks
        for (auto& surf : surfaces) {
            if (surf.is_focused) {
                if (event.type == InputType::POINTER_CLICK || event.type == InputType::POINTER_MOVE) {
                     // Bounding box hit test
                     if (event.x >= surf.x && event.x <= surf.x + surf.width &&
                         event.y >= surf.y && event.y <= surf.y + surf.height) {
                         surf.HandleInput(event);
                     } else if (event.type == InputType::POINTER_CLICK) {
                         std::cout << "[Compositor] Click outside bounds of focused window #" << surf.surface_id << "\n";
                     }
                } else {
                    // Keyboard events go straight to focused window
                    surf.HandleInput(event);
                }
                return;
            }
        }
    }

    void RenderFrame() {
        // KMS/DRM buffer swap logic goes here
    }
};

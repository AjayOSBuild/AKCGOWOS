#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>

enum class InputType { KEYBOARD, POINTER_MOVE, POINTER_CLICK };

struct InputEvent {
    InputType type;
    int key_code;
    int x, y;
    bool pressed;
};

struct HardwareCursor {
    int x = 0, y = 0;
    bool visible = true;
    void Move(int new_x, int new_y) { x = new_x; y = new_y; }
};

class FontRenderer {
private:
    std::string current_font_path;
public:
    bool LoadFont(const std::string& path) {
        current_font_path = path;
        std::cout << "[FontRenderer] Loaded UI font: " << path << "\n";
        return true;
    }
    void DrawText(const std::string& text, int x, int y, int size, uint32_t color) {}
};

class Graphics2D {
public:
    Graphics2D() { std::cout << "[Graphics2D] 2D vector graphics engine initialized.\n"; }
    void FillRect(int x, int y, int width, int height, uint32_t color) {}
    void DrawRect(int x, int y, int width, int height, uint32_t color, int thickness) {}
    void DrawLine(int x1, int y1, int x2, int y2, uint32_t color, int thickness) {}
};

struct WindowSurface {
    int surface_id;
    std::string title;
    int x, y, width, height;
    bool visible = true;
    int z_index = 0;
    bool is_focused = false;
    bool has_attached_buffer = false; // Tracks shared memory buffer state

    void Move(int new_x, int new_y) {
        x = new_x; y = new_y;
        std::cout << "[Compositor] Window #" << surface_id << " moved to (" << x << "," << y << ")\n";
    }

    void Resize(int new_width, int new_height) {
        width = new_width; height = new_height;
        std::cout << "[Compositor] Window #" << surface_id << " resized to " << width << "x" << height << "\n";
    }

    void HandleInput(const InputEvent& event) {
        if (event.type == InputType::KEYBOARD) {
            std::cout << "[Window #" << surface_id << "] Received KEY " << (event.pressed ? "DOWN" : "UP") << "\n";
        } else if (event.type == InputType::POINTER_CLICK) {
            std::cout << "[Window #" << surface_id << "] Received CLICK " << (event.pressed ? "DOWN" : "UP") << "\n";
        }
    }

    void Render(Graphics2D& gfx, FontRenderer& font_engine) {
        if (!visible) return;
        gfx.FillRect(x, y, width, height, 0xFFFFFFFF);
        uint32_t title_color = is_focused ? 0xFF333333 : 0xFF888888;
        gfx.FillRect(x, y, width, 30, title_color);
        gfx.DrawRect(x, y, width, height, 0xFF000000, 1);
        font_engine.DrawText(title, x + 10, y + 20, 14, 0xFFFFFFFF);
        
        // In full execution, if (has_attached_buffer), we composite the memfd pixels here.
    }
};

class CompositorEngine {
private:
    std::vector<WindowSurface> surfaces;
    int next_id = 1;
    HardwareCursor cursor;
    FontRenderer font_engine;
    Graphics2D gfx_engine;

    void SortSurfaces() {
        std::sort(surfaces.begin(), surfaces.end(), [](const WindowSurface& a, const WindowSurface& b) {
            return a.z_index < b.z_index;
        });
    }

public:
    CompositorEngine() {
        std::cout << "[Compositor] Graphics engine initialized.\n";
        font_engine.LoadFont("/usr/share/fonts/nexus-ui-regular.ttf");
    }

    int CreateWindow(const std::string& title, int x, int y, int width, int height) {
        WindowSurface surf{next_id++, title, x, y, width, height, true, 0, false, false};
        surfaces.push_back(surf);
        std::cout << "[Compositor] Created window surface #" << surf.surface_id << ": " << title << "\n";
        SetFocus(surf.surface_id);
        return surf.surface_id;
    }

    void SetFocus(int surface_id) {
        int highest_z = 0;
        for (auto& surf : surfaces) {
            if (surf.z_index > highest_z) { highest_z = surf.z_index; }
            surf.is_focused = false;
        }
        for (auto& surf : surfaces) {
            if (surf.surface_id == surface_id) {
                surf.is_focused = true;
                surf.z_index = highest_z + 1;
            }
        }
        SortSurfaces();
    }

    void AttachBuffer(int surface_id) {
        for (auto& surf : surfaces) {
            if (surf.surface_id == surface_id) {
                surf.has_attached_buffer = true;
                std::cout << "[Compositor] Buffer attached to window #" << surface_id << " (Zero-copy enabled)\n";
                return;
            }
        }
    }

    void DispatchInputEvent(const InputEvent& event) {
        if (event.type == InputType::POINTER_MOVE) { cursor.Move(event.x, event.y); }
        for (auto& surf : surfaces) {
            if (surf.is_focused) {
                if (event.type == InputType::POINTER_CLICK || event.type == InputType::POINTER_MOVE) {
                     if (event.x >= surf.x && event.x <= surf.x + surf.width &&
                         event.y >= surf.y && event.y <= surf.y + surf.height) {
                         surf.HandleInput(event);
                     }
                } else {
                    surf.HandleInput(event);
                }
                return;
            }
        }
    }

    void RenderFrame() {
        for (auto& surf : surfaces) { surf.Render(gfx_engine, font_engine); }
    }
};

// IPC Abstraction for Unix Domain Socket Protocol
class DisplayServerIPC {
private:
    CompositorEngine& engine;
public:
    DisplayServerIPC(CompositorEngine& eng) : engine(eng) {
        std::cout << "[DisplayServerIPC] Listening on /run/os/display.sock for client connections...\n";
    }

    void PollClientRequests() {
        // Placeholder: Non-blocking read from Unix Domain Socket.
        // E.g., if a client sent a "CreateSurface" message:
        // engine.CreateWindow("RemoteClientApp", 200, 200, 400, 300);
    }
};

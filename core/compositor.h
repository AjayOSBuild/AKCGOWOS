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
    }
};

// Font Rendering Abstraction
class FontRenderer {
private:
    std::string current_font_path;
public:
    bool LoadFont(const std::string& path) {
        current_font_path = path;
        std::cout << "[FontRenderer] Loaded UI font: " << path << "\n";
        return true;
    }

    void DrawText(const std::string& text, int x, int y, int size, uint32_t color) {
        // Suppress logging in render loop to avoid spam
    }
};

// 2D Graphics Abstraction (Prepares for Cairo/Skia integration)
class Graphics2D {
public:
    Graphics2D() {
        std::cout << "[Graphics2D] 2D vector graphics engine initialized.\n";
    }

    void FillRect(int x, int y, int width, int height, uint32_t color) {
        // Fills a rectangle with a solid color (AARRGGBB)
    }

    void DrawRect(int x, int y, int width, int height, uint32_t color, int thickness) {
        // Draws a rectangle outline
    }

    void DrawLine(int x1, int y1, int x2, int y2, uint32_t color, int thickness) {
        // Draws a vector line
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
            // Suppress hover spam
        }
    }

    void Render(Graphics2D& gfx, FontRenderer& font_engine) {
        if (!visible) return;
        
        // 1. Draw Window Background (White)
        gfx.FillRect(x, y, width, height, 0xFFFFFFFF);
        
        // 2. Draw Title Bar Background (Dark Gray if focused, Light Gray if unfocused)
        uint32_t title_color = is_focused ? 0xFF333333 : 0xFF888888;
        gfx.FillRect(x, y, width, 30, title_color);
        
        // 3. Draw Window Border (Black)
        gfx.DrawRect(x, y, width, height, 0xFF000000, 1);
        
        // 4. Draw Window Title Text (White)
        font_engine.DrawText(title, x + 10, y + 20, 14, 0xFFFFFFFF);
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
        std::cout << "[Compositor] Graphics engine initialized with 2D rendering support.\n";
        font_engine.LoadFont("/usr/share/fonts/nexus-ui-regular.ttf");
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

    WindowSurface* GetWindow(int surface_id) {
        for (auto& surf : surfaces) {
            if (surf.surface_id == surface_id) return &surf;
        }
        return nullptr;
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
        for (auto& surf : surfaces) {
            surf.Render(gfx_engine, font_engine);
        }
    }
};

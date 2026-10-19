#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

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
};

class CompositorEngine {
private:
    std::vector<WindowSurface> surfaces;
    int next_id = 1;

    void SortSurfaces() {
        // Sort back-to-front based on z_index for Painter's algorithm rendering
        std::sort(surfaces.begin(), surfaces.end(), [](const WindowSurface& a, const WindowSurface& b) {
            return a.z_index < b.z_index;
        });
    }

public:
    CompositorEngine() {
        std::cout << "[Compositor] Graphics engine initialized with Window Management.\n";
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
            surf.is_focused = false; // Remove focus from all
        }
        
        for (auto& surf : surfaces) {
            if (surf.surface_id == surface_id) {
                surf.is_focused = true;
                surf.z_index = highest_z + 1; // Bring to front
                std::cout << "[Compositor] Window #" << surface_id << " focused and brought to front (z_index=" << surf.z_index << ")\n";
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

    void RenderFrame() {
        // In full execution, this iterates through 'surfaces' and flushes buffer swaps via DRM/KMS
    }
};

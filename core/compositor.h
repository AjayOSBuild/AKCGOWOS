#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct WindowSurface {
    int surface_id;
    std::string title;
    int x, y, width, height;
    bool visible = true;
};

class CompositorEngine {
private:
    std::vector<WindowSurface> surfaces;
    int next_id = 1;

public:
    CompositorEngine() {
        std::cout << "[Compositor] Graphics engine initialized.\n";
    }

    int CreateWindow(const std::string& title, int x, int y, int width, int height) {
        WindowSurface surf{next_id++, title, x, y, width, height, true};
        surfaces.push_back(surf);
        std::cout << "[Compositor] Created window surface #" << surf.surface_id << ": " << title << "\n";
        return surf.surface_id;
    }

    void RenderFrame() {
        // In full execution, this flubs buffer swaps via DRM/KMS or VirtIO GPU.
        // For this foundation scaffold, we log compositing passes.
        // std::cout << "[Compositor] Rendering frame for " << surfaces.size() << " active surfaces.\n";
    }
};

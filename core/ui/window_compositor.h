#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

struct WindowNode {
    uint32_t surface_id;
    int x;
    int y;
    int width;
    int height;
    int z_index;
    bool is_visible;
};

class WindowCompositor {
private:
    std::vector<WindowNode> window_stack;
    int screen_width;
    int screen_height;
    int next_z_index = 10;

    void SortByZIndex();

public:
    WindowCompositor(int width = 1920, int height = 1080);

    // Registers a new window surface to the compositor scene
    void AddWindow(uint32_t surface_id, int x, int y, int width, int height);

    // Removes a window from the composition tree
    void RemoveWindow(uint32_t surface_id);

    // Moves a window to the top of the z-order stack (focuses it)
    void RaiseWindow(uint32_t surface_id);

    // Updates window position on the screen
    void MoveWindow(uint32_t surface_id, int x, int y);

    // Simulates the render pass blending all visible windows into the main framebuffer
    void RenderFrame() const;
    
    // Displays the current window stack hierarchy
    void DisplayWindowTree() const;
};

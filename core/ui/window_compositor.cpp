#include "window_compositor.h"

WindowCompositor::WindowCompositor(int width, int height) : screen_width(width), screen_height(height) {
    std::cout << "[Compositor] Initializing Window Compositor (" << width << "x" << height << " output)...\n";
}

void WindowCompositor::SortByZIndex() {
    std::sort(window_stack.begin(), window_stack.end(), [](const WindowNode& a, const WindowNode& b) {
        return a.z_index < b.z_index;
    });
}

void WindowCompositor::AddWindow(uint32_t surface_id, int x, int y, int width, int height) {
    window_stack.push_back({surface_id, x, y, width, height, next_z_index++, true});
    SortByZIndex();
    std::cout << "[Compositor] Added Surface ID " << surface_id << " at (" << x << "," << y << ") size " << width << "x" << height << "\n";
}

void WindowCompositor::RemoveWindow(uint32_t surface_id) {
    auto it = std::remove_if(window_stack.begin(), window_stack.end(), 
        [surface_id](const WindowNode& w) { return w.surface_id == surface_id; });
    
    if (it != window_stack.end()) {
        window_stack.erase(it, window_stack.end());
        std::cout << "[Compositor] Removed Surface ID " << surface_id << "\n";
    }
}

void WindowCompositor::RaiseWindow(uint32_t surface_id) {
    for (auto& w : window_stack) {
        if (w.surface_id == surface_id) {
            w.z_index = next_z_index++;
            std::cout << "[Compositor] Raised Surface ID " << surface_id << " to foreground.\n";
            break;
        }
    }
    SortByZIndex();
}

void WindowCompositor::MoveWindow(uint32_t surface_id, int x, int y) {
    for (auto& w : window_stack) {
        if (w.surface_id == surface_id) {
            w.x = x;
            w.y = y;
            std::cout << "[Compositor] Moved Surface ID " << surface_id << " to (" << x << "," << y << ").\n";
            break;
        }
    }
}

void WindowCompositor::RenderFrame() const {
    std::cout << "\n[Compositor] --- VSYNC: Commencing Render Pass ---\n";
    std::cout << "  -> Clearing framebuffer to background color (0x000000)...\n";
    
    // Render from bottom to top (Painter's Algorithm)
    for (const auto& w : window_stack) {
        if (!w.is_visible) continue;
        std::cout << "  -> Blending Surface " << w.surface_id << " at (" << w.x << "," << w.y 
                  << ") [Z:" << w.z_index << "] using alpha-blending pipeline.\n";
    }
    
    std::cout << "  -> [IPC] Swapped buffers. Frame displayed.\n";
}

void WindowCompositor::DisplayWindowTree() const {
    std::cout << "\n--- Window Hierarchy (Bottom to Top) ---\n";
    for (const auto& w : window_stack) {
        std::cout << "Surface ID: " << w.surface_id << " | Z-Index: " << w.z_index 
                  << " | Geom: " << w.width << "x" << w.height << "@" << w.x << "," << w.y << "\n";
    }
    std::cout << "----------------------------------------\n\n";
}

int main() {
    std::cout << "=== OS Project Window Compositor Utility ===\n";
    WindowCompositor comp;

    // Simulate mapping client surfaces (from UI-001) to screen coordinates
    comp.AddWindow(1, 100, 100, 900, 700); // Package GUI
    comp.AddWindow(2, 400, 300, 800, 600); // Terminal 1
    
    comp.DisplayWindowTree();
    comp.RenderFrame();

    // User clicks the Package GUI window, raising it to the top
    std::cout << "\n[Input Event] User clicked Surface 1.\n";
    comp.RaiseWindow(1);
    
    // User drags the Terminal window
    std::cout << "\n[Input Event] User dragged Surface 2.\n";
    comp.MoveWindow(2, 450, 350);

    comp.DisplayWindowTree();
    comp.RenderFrame();

    std::cout << "[+] Window Compositor testing completed successfully.\n";
    return 0;
}

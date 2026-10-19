#pragma once
#include <iostream>
#include <string>
#include <vector>

// Defines the interaction zones for a window's title bar
enum class WindowAction { NONE, MINIMIZE, MAXIMIZE, CLOSE, DRAG };

struct WindowDecorations {
    int surface_id;
    int x, y, width, height;
    std::string title;
    int titlebar_height = 30;
    
    // Hit testing for window controls
    WindowAction GetActionAt(int click_x, int click_y) {
        if (click_y >= y && click_y <= y + titlebar_height) {
            if (click_x >= x + width - 30) return WindowAction::CLOSE;
            if (click_x >= x + width - 60) return WindowAction::MAXIMIZE;
            if (click_x >= x + width - 90) return WindowAction::MINIMIZE;
            return WindowAction::DRAG;
        }
        return WindowAction::NONE;
    }
};

class WindowManagerUI {
private:
    std::vector<WindowDecorations> active_decorations;

public:
    WindowManagerUI() {
        std::cout << "[WindowManagerUI] Initializing window decoration and chrome renderer...\n";
    }

    void RegisterWindow(int surface_id, const std::string& title, int x, int y, int width, int height) {
        active_decorations.push_back({surface_id, x, y, width, height, title, 30});
    }

    void HandleClick(int x, int y) {
        // Iterate backwards (top-most windows first based on Painter's algorithm logic)
        for (auto it = active_decorations.rbegin(); it != active_decorations.rend(); ++it) {
            WindowAction action = it->GetActionAt(x, y);
            if (action != WindowAction::NONE) {
                std::cout << "[WindowManagerUI] Action triggered on window #" << it->surface_id << ": ";
                switch(action) {
                    case WindowAction::CLOSE: std::cout << "CLOSE\n"; break;
                    case WindowAction::MAXIMIZE: std::cout << "MAXIMIZE\n"; break;
                    case WindowAction::MINIMIZE: std::cout << "MINIMIZE\n"; break;
                    case WindowAction::DRAG: std::cout << "DRAG\n"; break;
                    default: break;
                }
                return; // Consume the click
            }
        }
    }

    void Render() {
        // In full execution, this submits drawing commands for the title bar, 
        // minimize/maximize/close buttons, and borders around the application's pixel buffer.
        // std::cout << "[WindowManagerUI] Rendering decorations for " << active_decorations.size() << " windows.\n";
    }
};

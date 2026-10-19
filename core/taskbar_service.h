#pragma once
#include <iostream>
#include <string>
#include <vector>

struct AppShortcut {
    std::string name;
    std::string icon_path;
    std::string exec_command;
    bool is_running = false;
};

class TaskbarService {
private:
    std::vector<AppShortcut> pinned_apps;
    int screen_width = 1024;
    int taskbar_height = 40;

public:
    TaskbarService() {
        std::cout << "[Taskbar] Initializing taskbar layout and pinned applications...\n";
        
        // Pin default core applications
        pinned_apps.push_back({"Terminal", "/usr/share/icons/terminal.png", "/bin/terminal", false});
        pinned_apps.push_back({"File Explorer", "/usr/share/icons/explorer.png", "/bin/explorer", false});
        pinned_apps.push_back({"Browser", "/usr/share/icons/browser.png", "/bin/browser", false});
    }

    void Render() {
        // In full execution, this submits drawing commands to the Compositor via IPC
        // Example: Draw solid rectangle at bottom of screen
        // gfx.FillRect(0, screen_height - taskbar_height, screen_width, taskbar_height, 0xFF222222);
        
        // std::cout << "[Taskbar] Rendering taskbar background and " << pinned_apps.size() << " app icons.\n";
    }

    void HandleClick(int x, int y) {
        // Calculate which app icon was clicked based on coordinates
        std::cout << "[Taskbar] Click detected at relative coordinates (" << x << "," << y << ").\n";
    }
};

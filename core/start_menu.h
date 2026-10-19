#pragma once
#include <iostream>
#include <string>
#include <vector>

struct AppEntry {
    std::string name;
    std::string category;
    std::string exec_command;
};

class StartMenu {
private:
    bool is_open = false;
    std::vector<AppEntry> installed_apps;

public:
    StartMenu() {
        std::cout << "[StartMenu] Initializing application index and launcher UI...\n";
        
        // Mock application index for testing
        installed_apps.push_back({"Terminal", "System", "/bin/terminal"});
        installed_apps.push_back({"File Explorer", "System", "/bin/explorer"});
        installed_apps.push_back({"System Settings", "Preferences", "/bin/settings"});
        installed_apps.push_back({"Browser", "Internet", "/bin/browser"});
    }

    void Toggle() {
        is_open = !is_open;
        std::cout << "[StartMenu] State changed to: " << (is_open ? "OPEN" : "CLOSED") << "\n";
    }

    bool IsOpen() const {
        return is_open;
    }

    void Search(const std::string& query) {
        if (!is_open) return;
        std::cout << "[StartMenu] Searching for: " << query << "\n";
        // In full implementation, this queries the File Indexer and Application Manager
    }

    void Render() {
        if (!is_open) return;
        // std::cout << "[StartMenu] Rendering start menu overlay with " << installed_apps.size() << " apps.\n";
    }
};

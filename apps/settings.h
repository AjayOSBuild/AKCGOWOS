#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

enum class SettingsCategory { DISPLAY, NETWORK, PERSONALIZATION, ABOUT };

class SettingsApp {
private:
    SettingsCategory active_category = SettingsCategory::DISPLAY;

    void SaveConfiguration(const std::string& key, const std::string& value) {
        std::cout << "[Settings] Saving configuration -> " << key << " = " << value << "\n";
        // In full execution, this submits an IPC request to the Configuration Service 
        // to atomically update /etc/os/system.toml or ~/.config/os/user.json
    }

public:
    SettingsApp() {
        std::cout << "[Settings] Initializing native System Settings application...\n";
    }

    void SwitchCategory(SettingsCategory category) {
        active_category = category;
        std::cout << "[Settings] Switched to category ID: " << static_cast<int>(category) << "\n";
    }

    void ApplyTheme(bool dark_mode) {
        SaveConfiguration("ui.theme.dark_mode", dark_mode ? "true" : "false");
        // Trigger IPC broadcast to all running apps to update their palettes
    }

    void ApplyDisplayResolution(int width, int height) {
        std::string res = std::to_string(width) + "x" + std::to_string(height);
        SaveConfiguration("display.resolution", res);
        // Trigger IPC to Compositor (GFX-001) to reconfigure DRM output
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Draw window background
        gfx.FillRect(0, 0, 800, 600, 0xFFFAFAFA);
        
        // Draw Sidebar
        gfx.FillRect(0, 0, 200, 600, 0xFFE0E0E0);
        gfx.DrawRect(0, 0, 200, 600, 0xFFCCCCCC, 1);
        
        font_engine.DrawText("Settings", 20, 30, 16, 0xFF000000);
        
        // Sidebar Items
        uint32_t active_color = 0xFF0066CC;
        uint32_t inactive_color = 0xFF333333;
        
        font_engine.DrawText("Display", 30, 80, 14, active_category == SettingsCategory::DISPLAY ? active_color : inactive_color);
        font_engine.DrawText("Network", 30, 120, 14, active_category == SettingsCategory::NETWORK ? active_color : inactive_color);
        font_engine.DrawText("Personalization", 30, 160, 14, active_category == SettingsCategory::PERSONALIZATION ? active_color : inactive_color);
        font_engine.DrawText("About", 30, 200, 14, active_category == SettingsCategory::ABOUT ? active_color : inactive_color);

        // Draw Content Area based on active category
        int content_x = 230;
        int content_y = 50;

        switch (active_category) {
            case SettingsCategory::DISPLAY:
                font_engine.DrawText("Display Configuration", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Resolution: 1920x1080 (Active)", content_x, content_y + 50, 14, 0xFF333333);
                font_engine.DrawText("Scale: 100%", content_x, content_y + 90, 14, 0xFF333333);
                break;
            case SettingsCategory::NETWORK:
                font_engine.DrawText("Network & Internet", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Status: Connected to Ethernet", content_x, content_y + 50, 14, 0xFF333333);
                break;
            case SettingsCategory::PERSONALIZATION:
                font_engine.DrawText("Personalization", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Theme: Light", content_x, content_y + 50, 14, 0xFF333333);
                break;
            case SettingsCategory::ABOUT:
                font_engine.DrawText("About Nexus OS", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Version: 1.0.0-alpha", content_x, content_y + 50, 14, 0xFF333333);
                font_engine.DrawText("Kernel: Custom Linux Build", content_x, content_y + 80, 14, 0xFF333333);
                break;
        }
    }
};

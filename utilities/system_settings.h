#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

enum class ControlPanelSection { APPEARANCE, POWER, SOUND, USERS };

class SystemSettingsUtility {
private:
    ControlPanelSection active_section = ControlPanelSection::APPEARANCE;
    std::unordered_map<std::string, std::string> preferences;

public:
    SystemSettingsUtility() {
        std::cout << "[SysSettings] Initializing Comprehensive System Settings Utility...\n";
        // Load default preference profiles
        preferences["theme"] = "Nexus-Dark";
        preferences["power_profile"] = "Balanced";
        preferences["audio_output"] = "Primary Analog Output";
        preferences["username"] = "default_user";
    }

    void SetPreference(const std::string& key, const std::string& val) {
        preferences[key] = val;
        std::cout << "[SysSettings] Configuration updated -> " << key << " = " << val << "\n";
    }

    std::string GetPreference(const std::string& key) const {
        auto it = preferences.find(key);
        if (it != preferences.end()) return it->second;
        return "";
    }

    void SwitchSection(ControlPanelSection section) {
        active_section = section;
        std::cout << "[SysSettings] Active settings section changed.\n";
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        gfx.FillRect(0, 0, 900, 700, 0xFFFAFAFA);
        
        // Sidebar Navigation
        gfx.FillRect(0, 0, 220, 700, 0xFFE0E0E0);
        gfx.DrawRect(0, 0, 220, 700, 0xFFCCCCCC, 1);
        font_engine.DrawText("Control Panel", 20, 30, 16, 0xFF000000);

        font_engine.DrawText("Appearance", 30, 90, 13, active_section == ControlPanelSection::APPEARANCE ? 0xFF0066CC : 0xFF333333);
        font_engine.DrawText("Power Management", 30, 130, 13, active_section == ControlPanelSection::POWER ? 0xFF0066CC : 0xFF333333);
        font_engine.DrawText("Sound & Audio", 30, 170, 13, active_section == ControlPanelSection::SOUND ? 0xFF0066CC : 0xFF333333);
        font_engine.DrawText("User Accounts", 30, 210, 13, active_section == ControlPanelSection::USERS ? 0xFF0066CC : 0xFF333333);

        // Content Area Header
        int content_x = 260;
        int content_y = 40;

        switch (active_section) {
            case ControlPanelSection::APPEARANCE:
                font_engine.DrawText("Appearance & Themes", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Current Theme: " + preferences["theme"], content_x, content_y + 50, 14, 0xFF333333);
                break;
            case ControlPanelSection::POWER:
                font_engine.DrawText("Power & Performance", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Active Profile: " + preferences["power_profile"], content_x, content_y + 50, 14, 0xFF333333);
                break;
            case ControlPanelSection::SOUND:
                font_engine.DrawText("Sound & Audio Devices", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Output Device: " + preferences["audio_output"], content_x, content_y + 50, 14, 0xFF333333);
                break;
            case ControlPanelSection::USERS:
                font_engine.DrawText("User Accounts & Groups", content_x, content_y, 18, 0xFF000000);
                font_engine.DrawText("Current Account: " + preferences["username"], content_x, content_y + 50, 14, 0xFF333333);
                break;
        }
    }
};

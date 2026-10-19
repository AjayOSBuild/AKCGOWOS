#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

struct HardwareNode {
    std::string name;
    std::string category; // "PCI", "Storage", "Network", "USB"
    std::string path;
    bool active = true;
};

class SystemDeviceManager {
private:
    std::vector<HardwareNode> hardware_devices;

public:
    SystemDeviceManager() {
        std::cout << "[SysDeviceManager] Initializing Advanced System Device Manager...\n";
        EnumerateHardware();
    }

    void EnumerateHardware() {
        hardware_devices.clear();

        // 1. Scan PCI Devices
        ScanSysfsCategory("/sys/bus/pci/devices", "PCI Device");

        // 2. Scan Storage Block Devices
        ScanSysfsCategory("/sys/block", "Storage (Block)");

        // 3. Scan Network Interfaces
        ScanSysfsCategory("/sys/class/net", "Network Interface");

        std::cout << "[SysDeviceManager] Enumeration complete. Discovered " << hardware_devices.size() << " device nodes.\n";
    }

    void ScanSysfsCategory(const std::string& sys_path, const std::string& category) {
        std::error_code ec;
        if (!fs::exists(sys_path, ec)) return;

        for (const auto& entry : fs::directory_iterator(sys_path, fs::directory_options::skip_permission_denied, ec)) {
            std::string name = entry.path().filename().string();
            if (name == "." || name == "..") continue;

            HardwareNode node;
            node.name = name;
            node.category = category;
            node.path = entry.path().string();
            node.active = true;

            hardware_devices.push_back(node);
        }
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        gfx.FillRect(0, 0, 900, 700, 0xFFFAFAFA);
        
        // Header Banner
        gfx.FillRect(20, 20, 860, 50, 0xFFFFFFFF);
        gfx.DrawRect(20, 20, 860, 50, 0xFFDDDDDD, 1);
        font_engine.DrawText("System Hardware Inventory & Device Manager", 35, 50, 14, 0xFF333333);

        // Table Headers
        gfx.FillRect(20, 90, 860, 30, 0xFFE0E0E0);
        font_engine.DrawText("Device Name", 30, 110, 12, 0xFF000000);
        font_engine.DrawText("Category", 350, 110, 12, 0xFF000000);
        font_engine.DrawText("Sysfs Node Path", 550, 110, 12, 0xFF000000);

        // Table Rows
        int row_y = 140;
        int count = 0;
        for (const auto& dev : hardware_devices) {
            font_engine.DrawText(dev.name, 30, row_y, 12, 0xFF333333);
            font_engine.DrawText(dev.category, 350, row_y, 12, 0xFF333333);
            font_engine.DrawText(dev.path, 550, row_y, 11, 0xFF666666);
            
            row_y += 24;
            count++;
            if (count > 22) break; // Viewport limit
        }
    }
};

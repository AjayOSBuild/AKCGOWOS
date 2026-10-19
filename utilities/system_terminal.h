#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

class SystemTerminalUtility {
private:
    std::vector<std::string> history;
    std::string prompt = "nexus-admin@os:~# ";

public:
    SystemTerminalUtility() {
        std::cout << "[SysTerminal] Initializing System Administration Terminal Utility...\n";
        history.push_back("Nexus OS System Administration Recovery Shell v1.0");
        history.push_back("Type 'help' for a list of available system maintenance commands.");
    }

    void ExecuteCommand(const std::string& cmd) {
        history.push_back(prompt + cmd);
        std::cout << "[SysTerminal] Executing command: " << cmd << "\n";
        
        if (cmd == "help") {
            history.push_back("Available commands:");
            history.push_back("  sysinfo    - Display kernel and memory stats");
            history.push_back("  services   - List active core system daemons");
            history.push_back("  dmesg      - Print kernel ring buffer diagnostics");
            history.push_back("  clear      - Clear terminal output history");
            history.push_back("  reboot     - Trigger system restart event");
        } else if (cmd == "sysinfo") {
            history.push_back("Nexus OS Kernel 6.x.y-generic | Architecture: x86_64");
            history.push_back("Uptime: 01:24:50 | User: root (UID 0)");
        } else if (cmd == "services") {
            history.push_back("[ACTIVE] ServiceManager, DeviceManager, SessionManager");
            history.push_back("[ACTIVE] StorageManager, NetworkManager, Compositor");
        } else if (cmd == "dmesg") {
            history.push_back("[ 0.000000] Linux version initialized.");
            history.push_back("[ 0.412000] PCI: Probing PCI hardware buses.");
            history.push_back("[ 1.205000] EXT4-fs: mounted filesystem with ordered data mode.");
        } else if (cmd == "clear") {
            history.clear();
        } else if (cmd == "reboot") {
            history.push_back("Sending system reboot signal via SystemEventService...");
        } else if (!cmd.empty()) {
            history.push_back("Command not recognized: " + cmd + ". Type 'help' for options.");
        }
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        gfx.FillRect(0, 0, 900, 700, 0xFF0F0F0F); // Deep dark admin console theme
        
        int start_y = 20;
        int max_lines = 600 / 18;
        int start_idx = std::max(0, static_cast<int>(history.size()) - max_lines);

        for (size_t i = start_idx; i < history.size(); ++i) {
            font_engine.DrawText(history[i], 15, start_y, 13, 0xFF00FF66); // Classic hacker green / admin phosphor text
            start_y += 18;
        }
    }
};

#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

struct ProcessInfo {
    int pid;
    std::string name;
    std::string state;
    unsigned long memory_kb;
};

class TaskManagerApp {
private:
    float cpu_usage_percent = 0.0f;
    unsigned long total_memory_kb = 0;
    unsigned long free_memory_kb = 0;
    std::vector<ProcessInfo> active_processes;

    void PollSystemStats() {
        // Read memory info from /proc/meminfo
        std::ifstream meminfo("/proc/meminfo");
        std::string line;
        while (std::getline(meminfo, line)) {
            std::istringstream iss(line);
            std::string key;
            unsigned long val;
            std::string unit;
            if (iss >> key >> val >> unit) {
                if (key == "MemTotal:") total_memory_kb = val;
                else if (key == "MemAvailable:" || key == "MemFree:") free_memory_kb = val;
            }
        }

        // Mock process listing traversal from /proc
        active_processes.clear();
        active_processes.push_back({1, "init", "S", 4500});
        active_processes.push_back({102, "service_manager", "S", 8200});
        active_processes.push_back({105, "compositor", "R", 34500});
        active_processes.push_back({204, "desktop_shell", "S", 19200});
        active_processes.push_back({301, "terminal", "R", 12400});
    }

public:
    TaskManagerApp() {
        std::cout << "[TaskManager] Initializing native Task Manager...\n";
        PollSystemStats();
    }

    void Refresh() {
        PollSystemStats();
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Draw window background
        gfx.FillRect(0, 0, 800, 600, 0xFFF5F5F5);
        
        // Draw header panels for CPU and Memory meters
        gfx.FillRect(20, 20, 360, 100, 0xFFFFFFFF);
        gfx.DrawRect(20, 20, 360, 100, 0xFFCCCCCC, 1);
        font_engine.DrawText("CPU Usage", 35, 45, 12, 0xFF333333);
        
        std::string cpu_text = std::to_string(static_cast<int>(cpu_usage_percent)) + "%";
        font_engine.DrawText(cpu_text, 300, 45, 16, 0xFF0066CC);

        gfx.FillRect(420, 20, 360, 100, 0xFFFFFFFF);
        gfx.DrawRect(420, 20, 360, 100, 0xFFCCCCCC, 1);
        font_engine.DrawText("Memory Usage", 435, 45, 12, 0xFF333333);
        
        unsigned long used_mem = total_memory_kb - free_memory_kb;
        std::string mem_text = std::to_string(used_mem / 1024) + " MB / " + std::to_string(total_memory_kb / 1024) + " MB";
        font_engine.DrawText(mem_text, 580, 45, 14, 0xFF0066CC);

        // Draw Process Table Header
        gfx.FillRect(20, 140, 760, 30, 0xFFE0E0E0);
        font_engine.DrawText("PID", 30, 160, 12, 0xFF000000);
        font_engine.DrawText("Process Name", 100, 160, 12, 0xFF000000);
        font_engine.DrawText("State", 400, 160, 12, 0xFF000000);
        font_engine.DrawText("Memory (KB)", 600, 160, 12, 0xFF000000);

        // Render Process Rows
        int row_y = 190;
        for (const auto& proc : active_processes) {
            font_engine.DrawText(std::to_string(proc.pid), 30, row_y, 12, 0xFF333333);
            font_engine.DrawText(proc.name, 100, row_y, 12, 0xFF333333);
            font_engine.DrawText(proc.state, 400, row_y, 12, 0xFF333333);
            font_engine.DrawText(std::to_string(proc.memory_kb), 600, row_y, 12, 0xFF333333);
            row_y += 25;
        }
    }
};

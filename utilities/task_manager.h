#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <signal.h>

namespace fs = std::filesystem;

struct DetailedProcessInfo {
    int pid;
    std::string name;
    char state;
    unsigned long memory_bytes;
    int priority;
};

class SystemTaskManager {
private:
    std::vector<DetailedProcessInfo> processes;
    unsigned long total_ram_bytes = 0;
    unsigned long free_ram_bytes = 0;

public:
    SystemTaskManager() {
        std::cout << "[SysTaskManager] Initializing Advanced System Task Manager...\n";
        RefreshSystemState();
    }

    void RefreshSystemState() {
        // 1. Parse /proc/meminfo
        std::ifstream meminfo("/proc/meminfo");
        std::string line;
        while (std::getline(meminfo, line)) {
            std::istringstream iss(line);
            std::string key;
            unsigned long val;
            std::string unit;
            if (iss >> key >> val >> unit) {
                if (key == "MemTotal:") total_ram_bytes = val * 1024;
                else if (key == "MemAvailable:" || key == "MemFree:") free_ram_bytes = val * 1024;
            }
        }

        // 2. Scan /proc for active processes
        processes.clear();
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator("/proc", fs::directory_options::skip_permission_denied, ec)) {
            if (entry.is_directory(ec)) {
                std::string filename = entry.path().filename().string();
                // Check if directory name is a valid PID (only digits)
                if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
                    int pid = std::stoi(filename);
                    DetailedProcessInfo proc = ParseProcessStat(pid);
                    if (!proc.name.empty()) {
                        processes.push_back(proc);
                    }
                }
            }
        }
    }

    DetailedProcessInfo ParseProcessStat(int pid) {
        DetailedProcessInfo proc;
        proc.pid = pid;
        proc.name = "Unknown";
        proc.state = '?';
        proc.memory_bytes = 0;
        proc.priority = 0;

        std::string stat_path = "/proc/" + std::to_string(pid) + "/stat";
        std::ifstream stat_file(stat_path);
        if (!stat_file.is_open()) return proc;

        std::string stat_content;
        std::getline(stat_file, stat_content);
        
        // Parse comm (process name inside parentheses)
        size_t start_idx = stat_content.find('(');
        size_t end_idx = stat_content.rfind(')');
        if (start_idx != std::string::npos && end_idx != std::string::npos && end_idx > start_idx) {
            proc.name = stat_content.substr(start_idx + 1, end_idx - start_idx - 1);
            
            // Parse remaining metrics after comm
            std::string remainder = stat_content.substr(end_idx + 2);
            std::istringstream iss(remainder);
            char state;
            int ppid, pgrp, session, tty_nr, tpgid;
            unsigned int flags;
            unsigned long minflt, cminflt, majflt, cmajflt, utime, stime;
            long cutime, cstime, priority, nice, num_threads, itrealvalue;
            unsigned long long starttime;
            unsigned long vsize;
            long rss;

            if (iss >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags
                    >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime
                    >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue
                    >> starttime >> vsize >> rss) {
                proc.state = state;
                proc.priority = priority;
                proc.memory_bytes = rss * 4096; // RSS pages to bytes (standard 4KB page)
            }
        }
        return proc;
    }

    bool TerminateProcess(int pid, bool force = false) {
        int sig = force ? SIGKILL : SIGTERM;
        std::cout << "[SysTaskManager] Sending signal " << (force ? "SIGKILL" : "SIGTERM") << " to PID " << pid << "\n";
        if (kill(pid, sig) == 0) {
            RefreshSystemState();
            return true;
        }
        std::cerr << "[!] Failed to terminate PID " << pid << "\n";
        return false;
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        gfx.FillRect(0, 0, 900, 700, 0xFFFAFAFA);
        
        // Memory Overview Banner
        gfx.FillRect(20, 20, 860, 60, 0xFFFFFFFF);
        gfx.DrawRect(20, 20, 860, 60, 0xFFDDDDDD, 1);
        unsigned long used_ram = total_ram_bytes - free_ram_bytes;
        std::string mem_summary = "System RAM: " + std::to_string(used_ram / (1024*1024)) + " MB / " + 
                                  std::to_string(total_ram_bytes / (1024*1024)) + " MB active.";
        font_engine.DrawText(mem_summary, 35, 55, 14, 0xFF333333);

        // Table Headers
        gfx.FillRect(20, 100, 860, 30, 0xFFE0E0E0);
        font_engine.DrawText("PID", 30, 120, 12, 0xFF000000);
        font_engine.DrawText("Process Name", 120, 120, 12, 0xFF000000);
        font_engine.DrawText("State", 450, 120, 12, 0xFF000000);
        font_engine.DrawText("Priority", 550, 120, 12, 0xFF000000);
        font_engine.DrawText("Memory (Bytes)", 700, 120, 12, 0xFF000000);

        // Table Rows
        int row_y = 150;
        int count = 0;
        for (const auto& proc : processes) {
            font_engine.DrawText(std::to_string(proc.pid), 30, row_y, 12, 0xFF333333);
            font_engine.DrawText(proc.name, 120, row_y, 12, 0xFF333333);
            font_engine.DrawText(std::string(1, proc.state), 450, row_y, 12, 0xFF333333);
            font_engine.DrawText(std::to_string(proc.priority), 550, row_y, 12, 0xFF333333);
            font_engine.DrawText(std::to_string(proc.memory_bytes), 700, row_y, 12, 0xFF333333);
            
            row_y += 22;
            count++;
            if (count > 25) break; // Limit viewable rows
        }
    }
};

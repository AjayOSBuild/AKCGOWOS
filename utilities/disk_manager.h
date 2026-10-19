#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/statvfs.h>

struct PartitionInfo {
    std::string device;
    std::string mount_point;
    std::string fs_type;
    unsigned long long total_bytes = 0;
    unsigned long long free_bytes = 0;
    unsigned long long used_bytes = 0;
    int usage_percent = 0;
};

class SystemDiskManager {
private:
    std::vector<PartitionInfo> partitions;

public:
    SystemDiskManager() {
        std::cout << "[SysDiskManager] Initializing Advanced System Disk Management Utility...\n";
        ScanPartitions();
    }

    void ScanPartitions() {
        partitions.clear();
        std::ifstream mounts("/proc/mounts");
        std::string line;

        while (std::getline(mounts, line)) {
            std::istringstream iss(line);
            std::string device, mount_point, fs_type, options;
            int dump, pass;

            if (iss >> device >> mount_point >> fs_type >> options >> dump >> pass) {
                // Filter out pseudo/virtual filesystems
                if (fs_type == "proc" || fs_type == "sysfs" || fs_type == "devtmpfs" || 
                    fs_type == "debugfs" || fs_type == "securityfs" || fs_type == "pstore") {
                    continue;
                }

                PartitionInfo part;
                part.device = device;
                part.mount_point = mount_point;
                part.fs_type = fs_type;

                // Query disk space usage via statvfs
                struct statvfs vfs;
                if (statvfs(mount_point.c_str(), &vfs) == 0) {
                    part.total_bytes = static_cast<unsigned long long>(vfs.f_blocks) * vfs.f_frsize;
                    part.free_bytes = static_cast<unsigned long long>(vfs.f_bavail) * vfs.f_frsize;
                    part.used_bytes = part.total_bytes > part.free_bytes ? (part.total_bytes - part.free_bytes) : 0;
                    
                    if (part.total_bytes > 0) {
                        part.usage_percent = static_cast<int>((static_cast<double>(part.used_bytes) / part.total_bytes) * 100.0);
                    }
                }

                partitions.push_back(part);
            }
        }
        std::cout << "[SysDiskManager] Scan complete. Discovered " << partitions.size() << " active mounted volumes.\n";
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        gfx.FillRect(0, 0, 900, 700, 0xFFFAFAFA);
        
        // Header Banner
        gfx.FillRect(20, 20, 860, 50, 0xFFFFFFFF);
        gfx.DrawRect(20, 20, 860, 50, 0xFFDDDDDD, 1);
        font_engine.DrawText("Storage Volumes & Disk Management", 35, 50, 14, 0xFF333333);

        // Table Headers
        gfx.FillRect(20, 90, 860, 30, 0xFFE0E0E0);
        font_engine.DrawText("Device", 30, 110, 12, 0xFF000000);
        font_engine.DrawText("Mount Point", 200, 110, 12, 0xFF000000);
        font_engine.DrawText("Filesystem", 400, 110, 12, 0xFF000000);
        font_engine.DrawText("Total Size", 550, 110, 12, 0xFF000000);
        font_engine.DrawText("Used (%)", 720, 110, 12, 0xFF000000);

        // Table Rows
        int row_y = 140;
        for (const auto& part : partitions) {
            font_engine.DrawText(part.device, 30, row_y, 12, 0xFF333333);
            font_engine.DrawText(part.mount_point, 200, row_y, 12, 0xFF333333);
            font_engine.DrawText(part.fs_type, 400, row_y, 12, 0xFF333333);
            
            unsigned long long total_mb = part.total_bytes / (1024 * 1024);
            font_engine.DrawText(std::to_string(total_mb) + " MB", 550, row_y, 12, 0xFF333333);
            
            std::string usage_str = std::to_string(part.usage_percent) + "%";
            font_engine.DrawText(usage_str, 720, row_y, 12, part.usage_percent > 90 ? 0xFFFF0000 : 0xFF0066CC);
            
            row_y += 28;
        }
    }
};

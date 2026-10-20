#include "disk_discovery.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iomanip>

namespace fs = std::filesystem;

DiskScanner::DiskScanner(const std::string& sysfs_root) : sys_block_path(sysfs_root) {
    std::cout << "[DiskScanner] Initializing strict read-only disk discovery subsystem...\n";
}

std::string DiskScanner::ReadSysfsFile(const std::string& path) const {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::string line;
    std::getline(file, line);
    // Trim whitespace
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    return line;
}

bool DiskScanner::ShouldSkipDevice(const std::string& name) const {
    if (name.find("loop") == 0) return true;
    if (name.find("ram") == 0) return true;
    if (name.find("sr") == 0) return true; // Optical drives
    return false;
}

std::vector<DiskInfo> DiskScanner::DiscoverDisks() const {
    std::vector<DiskInfo> disks;

    if (!fs::exists(sys_block_path)) {
        std::cerr << "[!] Warning: sysfs block path (" << sys_block_path << ") not found. Mocking QEMU disk for testing.\n";
        disks.push_back({"vda", "QEMU HARDDISK", 21474836480ULL, false, true}); // 20GB Virtual Disk
        return disks;
    }

    for (const auto& entry : fs::directory_iterator(sys_block_path)) {
        std::string name = entry.path().filename().string();
        
        if (ShouldSkipDevice(name)) continue;

        DiskInfo info;
        info.name = name;
        
        std::string size_str = ReadSysfsFile(entry.path().string() + "/size");
        // Linux sysfs size is in 512-byte sectors
        info.size_bytes = size_str.empty() ? 0 : std::stoull(size_str) * 512;
        
        std::string removable_str = ReadSysfsFile(entry.path().string() + "/removable");
        info.is_removable = (removable_str == "1");
        
        std::string model = ReadSysfsFile(entry.path().string() + "/device/model");
        info.model = model.empty() ? "Unknown Device" : model;
        
        info.is_virtual = (model.find("QEMU") != std::string::npos || name.find("vd") == 0);

        disks.push_back(info);
    }
    
    return disks;
}

void DiskScanner::PrintDisks(const std::vector<DiskInfo>& disks) const {
    std::cout << "\n======================================================\n";
    std::cout << "                 DISCOVERED DISK TARGETS              \n";
    std::cout << "======================================================\n";
    
    if (disks.empty()) {
        std::cout << "[!] No valid installation targets found.\n";
        return;
    }

    for (size_t i = 0; i < disks.size(); ++i) {
        const auto& d = disks[i];
        double size_gb = static_cast<double>(d.size_bytes) / (1024 * 1024 * 1024);
        
        std::cout << "[" << i << "] /dev/" << std::left << std::setw(6) << d.name 
                  << " | " << std::fixed << std::setprecision(2) << std::setw(7) << size_gb << " GB "
                  << "| " << std::setw(20) << d.model 
                  << (d.is_removable ? " [REMOVABLE]" : "")
                  << (d.is_virtual ? " [VIRTUAL]" : "") << "\n";
    }
    std::cout << "======================================================\n";
    std::cout << "CRITICAL: Nexus OS Installer is in READ-ONLY mode.\n";
}

int main() {
    std::cout << "=== OS Project Installer: Disk Discovery Utility ===\n";
    DiskScanner scanner;
    
    std::vector<DiskInfo> disks = scanner.DiscoverDisks();
    scanner.PrintDisks(disks);
    
    std::cout << "[+] Disk Discovery testing completed safely.\n";
    return 0;
}

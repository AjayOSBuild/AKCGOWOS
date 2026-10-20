#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct DiskInfo {
    std::string name;
    std::string model;
    uint64_t size_bytes;
    bool is_removable;
    bool is_virtual;
};

class DiskScanner {
private:
    std::string sys_block_path;

    std::string ReadSysfsFile(const std::string& path) const;
    bool ShouldSkipDevice(const std::string& name) const;

public:
    DiskScanner(const std::string& sysfs_root = "/sys/block");

    // Scans the system for available block devices (excluding loop/ram drives)
    std::vector<DiskInfo> DiscoverDisks() const;

    // Displays the discovered disks safely to the user
    void PrintDisks(const std::vector<DiskInfo>& disks) const;
};

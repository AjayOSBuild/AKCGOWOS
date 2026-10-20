#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct PartitionInfo {
    std::string name;
    uint64_t size_bytes;
    // Note: Filesystem discovery (e.g., reading magic bytes) is deferred to a future task
};

struct DiskInfo {
    std::string name;
    std::string model;
    uint64_t size_bytes;
    bool is_removable;
    bool is_virtual;
    std::vector<PartitionInfo> partitions;
};

class DiskScanner {
private:
    std::string sys_block_path;

    std::string ReadSysfsFile(const std::string& path) const;
    bool ShouldSkipDevice(const std::string& name) const;
    void DiscoverPartitions(const std::string& disk_path, DiskInfo& disk) const;

public:
    DiskScanner(const std::string& sysfs_root = "/sys/block");

    std::vector<DiskInfo> DiscoverDisks() const;
    void PrintDisks(const std::vector<DiskInfo>& disks) const;
};

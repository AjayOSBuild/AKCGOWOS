#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct PhysicalVolume {
    std::string device_path;
    uint64_t total_size_mb;
    uint64_t free_size_mb;
};

struct LogicalVolume {
    std::string name;
    uint64_t size_mb;
    std::string mount_point;
};

struct VolumeGroup {
    std::string name;
    std::vector<std::string> physical_volumes;
    std::unordered_map<std::string, LogicalVolume> logical_volumes;
    uint64_t total_capacity_mb = 0;
    uint64_t free_capacity_mb = 0;
};

class LvmManager {
private:
    std::unordered_map<std::string, PhysicalVolume> pvs;
    std::unordered_map<std::string, VolumeGroup> vgs;

public:
    LvmManager();

    bool CreatePhysicalVolume(const std::string& device_path, uint64_t size_mb);
    bool CreateVolumeGroup(const std::string& vg_name, const std::vector<std::string>& pv_paths);
    bool CreateLogicalVolume(const std::string& vg_name, const std::string& lv_name, uint64_t size_mb);
    void DisplayLayout() const;
};

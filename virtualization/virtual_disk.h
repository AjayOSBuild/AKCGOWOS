#pragma once
#include <string>

class VirtualDiskManager {
public:
    VirtualDiskManager();

    bool CreateDisk(const std::string& path, const std::string& size, const std::string& format = "qcow2") const;
    bool DiskExists(const std::string& path) const;

    // Snapshot management
    bool CreateSnapshot(const std::string& path, const std::string& snapshot_name) const;
    bool DeleteSnapshot(const std::string& path, const std::string& snapshot_name) const;
    
    // Restores the disk to a previously saved snapshot state
    bool RestoreSnapshot(const std::string& path, const std::string& snapshot_name) const;
};

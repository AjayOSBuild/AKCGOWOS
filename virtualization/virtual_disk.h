#pragma once
#include <string>

class VirtualDiskManager {
public:
    VirtualDiskManager();

    // Creates a new virtual disk image using qemu-img (defaulting to qcow2 format)
    bool CreateDisk(const std::string& path, const std::string& size, const std::string& format = "qcow2") const;

    // Checks if a disk image exists and is accessible
    bool DiskExists(const std::string& path) const;
};

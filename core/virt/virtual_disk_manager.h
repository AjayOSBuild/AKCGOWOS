#pragma once
#include <iostream>
#include <string>
#include <vector>

enum class DiskFormat {
    RAW,
    QCOW2,
    UNKNOWN
};

struct DiskImageInfo {
    DiskFormat format;
    uint64_t virtual_size;
    std::string backing_file;
    bool is_valid;
};

class VirtualDiskManager {
public:
    VirtualDiskManager();

    // Parses the image header to determine format and metadata
    DiskImageInfo InspectImage(const std::string& image_path);

    // Mounts the virtual disk image to a local directory via loopback or NBD
    bool MountImage(const std::string& image_path, const std::string& mount_point);

    // Unmounts the virtual disk and detaches block devices
    bool UnmountImage(const std::string& mount_point);
};

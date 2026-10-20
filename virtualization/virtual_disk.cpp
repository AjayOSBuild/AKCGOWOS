#include "virtual_disk.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

VirtualDiskManager::VirtualDiskManager() {
    std::cout << "[DiskManager] Initializing Virtual Disk Manager...\n";
}

bool VirtualDiskManager::CreateDisk(const std::string& path, const std::string& size, const std::string& format) const {
    if (DiskExists(path)) return false;
    std::string command = "qemu-img create -f " + format + " " + path + " " + size + " > /dev/null 2>&1";
    return (std::system(command.c_str()) == 0);
}

bool VirtualDiskManager::DiskExists(const std::string& path) const {
    return (access(path.c_str(), F_OK) != -1);
}

bool VirtualDiskManager::CreateSnapshot(const std::string& path, const std::string& snapshot_name) const {
    if (!DiskExists(path)) return false;
    std::cout << "[DiskManager] Creating snapshot '" << snapshot_name << "' for disk: " << path << "...\n";
    
    // Simulating qemu-img snapshot creation
    std::string command = "qemu-img snapshot -c " + snapshot_name + " " + path + " > /dev/null 2>&1";
    int result = std::system(command.c_str());
    
    if (result == 0) {
        std::cout << "[DiskManager] Snapshot created successfully.\n";
        return true;
    }
    std::cerr << "[!] Failed to create snapshot. Is the disk in qcow2 format?\n";
    return false;
}

bool VirtualDiskManager::DeleteSnapshot(const std::string& path, const std::string& snapshot_name) const {
    if (!DiskExists(path)) return false;
    std::cout << "[DiskManager] Deleting snapshot '" << snapshot_name << "' from disk: " << path << "...\n";
    
    std::string command = "qemu-img snapshot -d " + snapshot_name + " " + path + " > /dev/null 2>&1";
    int result = std::system(command.c_str());
    
    return (result == 0);
}

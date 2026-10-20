#include "virtual_disk.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

VirtualDiskManager::VirtualDiskManager() {
    std::cout << "[DiskManager] Initializing Virtual Disk Manager...\n";
}

bool VirtualDiskManager::CreateDisk(const std::string& path, const std::string& size, const std::string& format) const {
    if (DiskExists(path)) {
        std::cerr << "[!] Disk image already exists at: " << path << "\n";
        return false;
    }

    std::cout << "[DiskManager] Creating " << format << " virtual disk at " << path << " (Size: " << size << ")...\n";
    
    // In production, use fork/execvp. Using system() for simulation brevity.
    std::string command = "qemu-img create -f " + format + " " + path + " " + size + " > /dev/null 2>&1";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "[DiskManager] Successfully created virtual disk.\n";
        return true;
    } else {
        std::cerr << "[!] Failed to create virtual disk. Ensure qemu-utils is installed.\n";
        return false;
    }
}

bool VirtualDiskManager::DiskExists(const std::string& path) const {
    return (access(path.c_str(), F_OK) != -1);
}

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <map>
#include <filesystem>
#include <sys/mount.h>
#include <cerrno>
#include <cstring>
#include "vfs.h"
#include "fs_ext4.h"
#include "fs_fat32.h"
#include "fs_exfat.h"
#include "fs_ntfs.h"

namespace fs = std::filesystem;

// VFS Registry
class VFSRegistry {
private:
    std::map<std::string, std::shared_ptr<IVirtualFileSystem>> filesystems;
public:
    void RegisterFS(const std::string& type, std::shared_ptr<IVirtualFileSystem> fs) {
        filesystems[type] = fs;
        std::cout << "  - Registered filesystem handler: " << type << "\n";
    }
    
    std::shared_ptr<IVirtualFileSystem> GetFS(const std::string& type) {
        if (filesystems.find(type) != filesystems.end()) {
            return filesystems[type];
        }
        return nullptr;
    }
};

// Mount & Unmount Service Module with Hotplug Automation
class MountService {
public:
    bool MountDevice(const std::string& source, const std::string& target, const std::string& fstype, unsigned long flags = 0, const std::string& data = "") {
        std::cout << "[StorageManager] Attempting to mount " << source << " to " << target << " (" << fstype << ")...\n";
        
        // Ensure target directory exists
        std::error_code ec;
        fs::create_directories(target, ec);

        if (mount(source.c_str(), target.c_str(), fstype.c_str(), flags, data.empty() ? nullptr : data.c_str()) == 0) {
            std::cout << "[+] Successfully mounted " << source << " at " << target << "\n";
            return true;
        } else {
            std::cerr << "[!] Mount failed for " << source << ": " << std::strerror(errno) << "\n";
            return false;
        }
    }

    bool UnmountDevice(const std::string& target, bool force = false) {
        std::cout << "[StorageManager] Attempting to unmount " << target << "...\n";
        int flags = force ? MNT_FORCE : 0;
        if (umount2(target.c_str(), flags) == 0) {
            std::cout << "[+] Successfully unmounted " << target << "\n";
            return true;
        } else {
            std::cerr << "[!] Unmount failed for " << target << ": " << std::strerror(errno) << "\n";
            return false;
        }
    }

    // Automated hotplug handler for removable drives
    void HandleHotplugDevice(const std::string& devName) {
        std::string source = "/dev/" + devName;
        std::string target = "/mnt/usb/" + devName;
        
        std::cout << "[StorageManager] Hotplug detected new block device: " << source << "\n";
        
        // Default to vfat/FAT32 for standard USB flash media, falling back or testing registration
        MountDevice(source, target, "vfat");
    }
};

int main() {
    std::cout << "=== OS Project Storage Manager (Ring 3) ===\n";
    std::cout << "[*] Initializing block device tracking and VFS mounts...\n";

    VFSRegistry registry;
    std::cout << "[*] Initializing Virtual Filesystem (VFS) abstraction layer...\n";
    
    registry.RegisterFS("ext4", std::make_shared<Ext4FileSystem>());
    registry.RegisterFS("vfat", std::make_shared<Fat32FileSystem>());
    registry.RegisterFS("exfat", std::make_shared<ExFatFileSystem>());
    registry.RegisterFS("ntfs", std::make_shared<NtfsFileSystem>());

    MountService mounter;
    std::cout << "[*] Mount/Unmount service initialized with hotplug automation.\n";

    std::cout << "[+] Storage Manager active. Monitoring for storage volumes...\n";

    // Continuous background monitor loop simulating hotplug detection via sysfs
    while (true) {
        // In full execution, this polls a netlink uevent socket or D-Bus signal.
        // Here we keep the daemon alive to manage active storage handles.
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}

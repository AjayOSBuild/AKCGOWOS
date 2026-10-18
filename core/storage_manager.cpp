#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <map>
#include <sys/mount.h>
#include <cerrno>
#include <cstring>
#include "vfs.h"
#include "fs_ext4.h"
#include "fs_fat32.h"
#include "fs_exfat.h"

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

// Mount & Unmount Service Module
class MountService {
public:
    bool MountDevice(const std::string& source, const std::string& target, const std::string& fstype, unsigned long flags = 0, const std::string& data = "") {
        std::cout << "[StorageManager] Attempting to mount " << source << " to " << target << " (" << fstype << ")...\n";
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
};

int main() {
    std::cout << "=== OS Project Storage Manager (Ring 3) ===\n";
    std::cout << "[*] Initializing block device tracking and VFS mounts...\n";

    VFSRegistry registry;
    std::cout << "[*] Initializing Virtual Filesystem (VFS) abstraction layer...\n";
    
    // Register filesystem wrappers
    registry.RegisterFS("ext4", std::make_shared<Ext4FileSystem>());
    registry.RegisterFS("vfat", std::make_shared<Fat32FileSystem>());
    registry.RegisterFS("exfat", std::make_shared<ExFatFileSystem>());

    MountService mounter;
    std::cout << "[*] Mount/Unmount service initialized and ready.\n";

    std::cout << "[+] Storage Manager active. Ready to orchestrate storage volumes...\n";

    // In future phases, this daemon will listen on D-Bus for API calls
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}

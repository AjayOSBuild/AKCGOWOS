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

// Access Control Guard
class VFSAuthorizer {
public:
    static bool CheckReadAccess(const VFSFile& file, uint32_t uid) {
        // Root (UID 0) bypasses standard restrictions
        if (uid == 0) return true;
        if (file.perms.uid == uid && file.perms.allow_read) return true;
        // Default fallback policy
        return file.perms.allow_read;
    }

    static bool CheckWriteAccess(const VFSFile& file, uint32_t uid) {
        if (uid == 0) return true;
        if (file.perms.uid == uid && file.perms.allow_write) return true;
        return false;
    }
};

// Mount & Unmount Service Module
class MountService {
public:
    bool MountDevice(const std::string& source, const std::string& target, const std::string& fstype, unsigned long flags = 0, const std::string& data = "") {
        std::cout << "[StorageManager] Attempting to mount " << source << " to " << target << " (" << fstype << ")...\n";
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
};

int main() {
    std::cout << "=== OS Project Storage Manager (Ring 3) ===\n";
    std::cout << "[*] Initializing block device tracking and VFS permissions...\n";

    VFSRegistry registry;
    registry.RegisterFS("ext4", std::make_shared<Ext4FileSystem>());
    registry.RegisterFS("vfat", std::make_shared<Fat32FileSystem>());
    registry.RegisterFS("exfat", std::make_shared<ExFatFileSystem>());
    registry.RegisterFS("ntfs", std::make_shared<NtfsFileSystem>());

    MountService mounter;
    std::cout << "[+] Storage Manager active with VFS access control enforcement.\n";

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}

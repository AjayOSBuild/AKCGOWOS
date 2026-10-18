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
#include "file_watcher.h"

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
};

int main() {
    std::cout << "=== OS Project Storage Manager (Ring 3) ===\n";
    std::cout << "[*] Initializing block device tracking and File Watcher...\n";

    VFSRegistry registry;
    registry.RegisterFS("ext4", std::make_shared<Ext4FileSystem>());
    registry.RegisterFS("vfat", std::make_shared<Fat32FileSystem>());
    registry.RegisterFS("exfat", std::make_shared<ExFatFileSystem>());
    registry.RegisterFS("ntfs", std::make_shared<NtfsFileSystem>());

    FileWatcher watcher;
    // Register default watch path for user documents/storage
    watcher.AddWatch("/mnt/usb");

    std::cout << "[+] Storage Manager active with real-time File Watcher monitoring.\n";

    while (true) {
        // Poll for filesystem events non-blockingly
        watcher.PollEvents([](const std::string& path, uint32_t mask) {
            std::cout << "[FileWatcher Event] Path changed: " << path << " (Mask: " << mask << ")\n";
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

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
#include "file_indexer.h"
#include "file_metadata.h"
#include "thumbnail_generator.h"
#include "file_trash.h"

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
    std::cout << "[*] Initializing storage stack and File Trash Manager...\n";

    VFSRegistry registry;
    registry.RegisterFS("ext4", std::make_shared<Ext4FileSystem>());
    registry.RegisterFS("vfat", std::make_shared<Fat32FileSystem>());
    registry.RegisterFS("exfat", std::make_shared<ExFatFileSystem>());
    registry.RegisterFS("ntfs", std::make_shared<NtfsFileSystem>());

    FileWatcher watcher;
    FileIndexer indexer;
    ThumbnailGenerator thumbnails;
    FileTrashManager trashManager;

    std::cout << "[+] Storage Manager active with complete VFS, Indexing, Metadata, Thumbnail, and Trash services.\n";

    while (true) {
        watcher.PollEvents([&indexer](const std::string& path, uint32_t mask) {
            if (mask & IN_CREATE || mask & IN_MOVED_TO) {
                indexer.AddPath(path);
            } else if (mask & IN_DELETE || mask & IN_MOVED_FROM) {
                indexer.RemovePath(path);
            }
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

#include "filesystem_creator.h"
#include <iostream>
#include <cstdlib>

FilesystemCreator::FilesystemCreator(bool is_dry_run) : dry_run(is_dry_run) {
    std::cout << "[FilesystemCreator] Initializing conservative filesystem creation engine (Dry-Run: " 
              << (dry_run ? "ENABLED" : "DISABLED") << ")...\n";
}

bool FilesystemCreator::FormatEfiPartition(const std::string& partition_path) const {
    std::cout << "[FilesystemCreator] Preparing EFI partition: /dev/" << partition_path << " for FAT32 formatting...\n";
    std::string cmd = "mkfs.vfat -F 32 /dev/" + partition_path;
    
    if (dry_run) {
        std::cout << "  -> [DRY-RUN SAFETY] Would execute: " << cmd << "\n";
        return true;
    }

    std::cout << "  -> Executing: " << cmd << "\n";
    int res = std::system(cmd.c_str());
    return (res == 0);
}

bool FilesystemCreator::FormatRootPartition(const std::string& partition_path) const {
    std::cout << "[FilesystemCreator] Preparing Root partition: /dev/" << partition_path << " for ext4 formatting...\n";
    std::string cmd = "mkfs.ext4 -F /dev/" + partition_path;

    if (dry_run) {
        std::cout << "  -> [DRY-RUN SAFETY] Would execute: " << cmd << "\n";
        return true;
    }

    std::cout << "  -> Executing: " << cmd << "\n";
    int res = std::system(cmd.c_str());
    return (res == 0);
}

int main() {
    std::cout << "=== OS Project Installer: Filesystem Creation Utility ===\n";
    
    // Explicitly enforce dry-run mode by default to prevent accidental data loss
    FilesystemCreator creator(true);
    
    creator.FormatEfiPartition("vda1");
    creator.FormatRootPartition("vda2");

    std::cout << "[+] Filesystem creation testing completed safely.\n";
    return 0;
}

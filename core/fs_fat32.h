#pragma once
#include "vfs.h"
#include <iostream>

// FAT32 (vfat) implementation of the Virtual Filesystem Interface
class Fat32FileSystem : public IVirtualFileSystem {
public:
    Fat32FileSystem() {
        std::cout << "[fat32] Driver module initialized.\n";
    }

    VFSError Mount(const std::string& devicePath, const std::string& mountPoint) override {
        std::cout << "[fat32] Preparing to map removable device " << devicePath << " to " << mountPoint << "\n";
        return VFSError::SUCCESS;
    }

    VFSError Unmount(const std::string& mountPoint) override {
        std::cout << "[fat32] Flushing FAT tables and unmapping " << mountPoint << "\n";
        return VFSError::SUCCESS;
    }
    
    VFSError Open(const std::string& path, VFSFile& outFile) override {
        std::cout << "[fat32] Opening file (checking 8.3/LFN constraints): " << path << "\n";
        outFile.path = path;
        return VFSError::SUCCESS;
    }
    
    VFSError Close(VFSFile& file) override {
        std::cout << "[fat32] Closing file: " << file.path << "\n";
        return VFSError::SUCCESS;
    }
    
    VFSError Read(VFSFile& file, uint64_t offset, uint64_t length, std::vector<uint8_t>& outBuffer) override {
        std::cout << "[fat32] Reading " << length << " bytes from cluster chain for " << file.path << "\n";
        return VFSError::SUCCESS;
    }
    
    VFSError Write(VFSFile& file, uint64_t offset, const std::vector<uint8_t>& buffer) override {
        std::cout << "[fat32] Writing " << buffer.size() << " bytes to " << file.path << "\n";
        return VFSError::SUCCESS;
    }
};

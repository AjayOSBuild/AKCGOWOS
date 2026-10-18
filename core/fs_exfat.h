#pragma once
#include "vfs.h"
#include <iostream>

// exFAT implementation of the Virtual Filesystem Interface
class ExFatFileSystem : public IVirtualFileSystem {
public:
    ExFatFileSystem() {
        std::cout << "[exfat] Driver module initialized.\n";
    }

    VFSError Mount(const std::string& devicePath, const std::string& mountPoint) override {
        std::cout << "[exfat] Preparing to map high-capacity device " << devicePath << " to " << mountPoint << "\n";
        return VFSError::SUCCESS;
    }

    VFSError Unmount(const std::string& mountPoint) override {
        std::cout << "[exfat] Flushing bitmap and unmapping " << mountPoint << "\n";
        return VFSError::SUCCESS;
    }
    
    VFSError Open(const std::string& path, VFSFile& outFile) override {
        std::cout << "[exfat] Opening file: " << path << "\n";
        outFile.path = path;
        return VFSError::SUCCESS;
    }
    
    VFSError Close(VFSFile& file) override {
        std::cout << "[exfat] Closing file: " << file.path << "\n";
        return VFSError::SUCCESS;
    }
    
    VFSError Read(VFSFile& file, uint64_t offset, uint64_t length, std::vector<uint8_t>& outBuffer) override {
        std::cout << "[exfat] Reading " << length << " bytes from exFAT stream for " << file.path << "\n";
        return VFSError::SUCCESS;
    }
    
    VFSError Write(VFSFile& file, uint64_t offset, const std::vector<uint8_t>& buffer) override {
        std::cout << "[exfat] Writing " << buffer.size() << " bytes to exFAT stream for " << file.path << "\n";
        return VFSError::SUCCESS;
    }
};

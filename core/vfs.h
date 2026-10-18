#pragma once
#include <string>
#include <vector>
#include <cstdint>

// Standardized VFS Error Codes
enum class VFSError {
    SUCCESS = 0,
    ACCESS_DENIED,
    NOT_FOUND,
    IO_ERROR,
    UNSUPPORTED_FS
};

// File descriptor abstraction
struct VFSFile {
    std::string path;
    uint64_t size;
    bool is_directory;
    void* internal_handle; // Filesystem-specific pointer
};

// The core Virtual Filesystem Interface
class IVirtualFileSystem {
public:
    virtual ~IVirtualFileSystem() = default;

    virtual VFSError Mount(const std::string& devicePath, const std::string& mountPoint) = 0;
    virtual VFSError Unmount(const std::string& mountPoint) = 0;
    
    virtual VFSError Open(const std::string& path, VFSFile& outFile) = 0;
    virtual VFSError Close(VFSFile& file) = 0;
    
    virtual VFSError Read(VFSFile& file, uint64_t offset, uint64_t length, std::vector<uint8_t>& outBuffer) = 0;
    virtual VFSError Write(VFSFile& file, uint64_t offset, const std::vector<uint8_t>& buffer) = 0;
};

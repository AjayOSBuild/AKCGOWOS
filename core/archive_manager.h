#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct ArchiveEntryInfo {
    std::string path;
    std::uint64_t size_bytes = 0;
    bool is_directory = false;
};

struct ArchiveResult {
    bool success = false;
    std::string message;
    std::size_t entries_processed = 0;

    explicit operator bool() const {
        return success;
    }
};

class ArchiveManager {
public:
    ArchiveManager();
    ~ArchiveManager();

    ArchiveManager(const ArchiveManager&) = delete;
    ArchiveManager& operator=(const ArchiveManager&) = delete;

    bool IsSupportedArchive(
        const std::string& archive_path) const;

    ArchiveResult ListEntries(
        const std::string& archive_path,
        std::vector<ArchiveEntryInfo>& out_entries) const;

    ArchiveResult Extract(
        const std::string& archive_path,
        const std::string& destination_directory,
        bool overwrite_existing = false) const;
};

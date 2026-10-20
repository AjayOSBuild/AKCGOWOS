#pragma once
#include <string>

class FilesystemCreator {
private:
    bool dry_run;

public:
    explicit FilesystemCreator(bool is_dry_run = true);

    // Formats the EFI partition as FAT32 safely
    bool FormatEfiPartition(const std::string& partition_path) const;

    // Formats the root partition as ext4 safely
    bool FormatRootPartition(const std::string& partition_path) const;
};

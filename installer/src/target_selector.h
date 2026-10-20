#pragma once
#include "disk_discovery.h"
#include <string>
#include <vector>

struct InstallationTarget {
    std::string disk_name;
    uint64_t total_size_bytes;
    bool user_confirmed;
};

class TargetSelector {
private:
    const uint64_t MIN_REQUIRED_BYTES = 10ULL * 1024 * 1024 * 1024; // 10 GB minimum requirement

public:
    TargetSelector();

    // Validates whether a selected disk meets minimum capacity and safety criteria
    bool ValidateTarget(const DiskInfo& disk, std::string& out_reason) const;

    // Simulates selecting and confirming a target disk safely
    bool SelectTarget(const std::vector<DiskInfo>& disks, size_t selected_index, InstallationTarget& out_target) const;
};

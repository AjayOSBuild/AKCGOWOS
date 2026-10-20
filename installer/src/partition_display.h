#pragma once
#include "disk_discovery.h"
#include <vector>
#include <string>

class PartitionDisplay {
public:
    PartitionDisplay();

    // Renders a clear, human-readable visual breakdown of disks and partitions
    void RenderLayout(const std::vector<DiskInfo>& disks) const;

    // Explains a proposed installation or partitioning operation before any modification
    void ExplainSafeOperation(const std::string& target_disk, const std::string& action_desc) const;
};

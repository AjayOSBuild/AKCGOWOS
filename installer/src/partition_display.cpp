#include "partition_display.h"
#include <iostream>
#include <iomanip>
#include <cmath>

PartitionDisplay::PartitionDisplay() {
    std::cout << "[PartitionDisplay] Initializing safe storage layout visualizer...\n";
}

void PartitionDisplay::RenderLayout(const std::vector<DiskInfo>& disks) const {
    std::cout << "\n======================================================================\n";
    std::cout << "                 NEXUS OS - STORAGE LAYOUT VIEWER                     \n";
    std::cout << "======================================================================\n";

    if (disks.empty()) {
        std::cout << "[!] No storage devices available to display.\n";
        return;
    }

    for (size_t i = 0; i < disks.size(); ++i) {
        const auto& d = disks[i];
        double disk_gb = static_cast<double>(d.size_bytes) / (1024 * 1024 * 1024);

        std::cout << "\nDisk [" << i << "]: /dev/" << d.name 
                  << " (" << std::fixed << std::setprecision(2) << disk_gb << " GB) - " << d.model;
        
        if (d.is_removable) std::cout << " [REMOVABLE]";
        if (d.is_virtual) std::cout << " [VIRTUAL]";
        std::cout << "\n";
        
        std::cout << " ├── [MBR/GPT Partition Table Header]\n";

        if (d.partitions.empty()) {
            std::cout << " └── (Unpartitioned Space / Empty Drive)\n";
        } else {
            for (size_t p = 0; p < d.partitions.size(); ++p) {
                const auto& part = d.partitions[p];
                double part_gb = static_cast<double>(part.size_bytes) / (1024 * 1024 * 1024);
                
                std::string connector = (p == d.partitions.size() - 1) ? " └── " : " ├── ";
                std::cout << connector << "/dev/" << std::left << std::setw(8) << part.name 
                          << " | Size: " << std::fixed << std::setprecision(2) << std::setw(6) << part_gb << " GB "
                          << " | Status: Protected / Detected\n";
            }
        }
    }
    std::cout << "\n======================================================================\n";
}

void PartitionDisplay::ExplainSafeOperation(const std::string& target_disk, const std::string& action_desc) const {
    std::cout << "\n**********************************************************************\n";
    std::cout << " [SAFE INSTALLER WARNING] CONFIRMATION REQUIRED BEFORE WRITE\n";
    std::cout << "**********************************************************************\n";
    std::cout << " Target Device : /dev/" << target_disk << "\n";
    std::cout << " Proposed Action: " << action_desc << "\n";
    std::cout << " Safety Policy : NO automatic destruction or repartitioning will occur\n";
    std::cout << "                 without explicit user override.\n";
    std::cout << "**********************************************************************\n";
}

int main() {
    std::cout << "=== OS Project Installer: Partition Display Utility ===\n";
    
    // Test using the DiskScanner to pull data and pass it to PartitionDisplay
    DiskScanner scanner;
    std::vector<DiskInfo> disks = scanner.DiscoverDisks();

    PartitionDisplay display;
    display.RenderLayout(disks);

    if (!disks.empty()) {
        display.ExplainSafeOperation(disks[0].name, "Format root partition as ext4 and install Nexus OS bootloader.");
    }

    std::cout << "\n[+] Partition Display testing completed safely.\n";
    return 0;
}

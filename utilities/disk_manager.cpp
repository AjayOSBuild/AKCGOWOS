#include <iostream>
#include "disk_manager.h"

int main() {
    std::cout << "=== OS Project Advanced System Disk Management Utility ===\n";
    SystemDiskManager disk_mgr;
    
    // Rescan partitions to verify mounts and statvfs parsing
    disk_mgr.ScanPartitions();
    
    std::cout << "[+] Advanced Disk Management utility initialized successfully.\n";
    return 0;
}

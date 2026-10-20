#include "target_selector.h"
#include <iostream>
#include <iomanip>

TargetSelector::TargetSelector() {
    std::cout << "[TargetSelector] Initializing conservative installation target validator...\n";
}

bool TargetSelector::ValidateTarget(const DiskInfo& disk, std::string& out_reason) const {
    if (disk.size_bytes < MIN_REQUIRED_BYTES) {
        out_reason = "Disk size is below the minimum requirement of 10 GB.";
        return false;
    }

    if (disk.is_removable) {
        out_reason = "Selected disk is marked as removable. Installing to USB drives/media is blocked for stability.";
        return false;
    }

    out_reason = "Target disk is valid and meets safety requirements.";
    return true;
}

bool TargetSelector::SelectTarget(const std::vector<DiskInfo>& disks, size_t selected_index, InstallationTarget& out_target) const {
    if (selected_index >= disks.size()) {
        std::cerr << "[!] Error: Invalid disk selection index.\n";
        return false;
    }

    const DiskInfo& chosen_disk = disks[selected_index];
    std::string validation_reason;

    std::cout << "[TargetSelector] Evaluating /dev/" << chosen_disk.name << " for installation...\n";
    
    if (!ValidateTarget(chosen_disk, validation_reason)) {
        std::cerr << "[!] Target Validation Failed: " << validation_reason << "\n";
        return false;
    }

    std::cout << "[+] Validation Passed: " << validation_reason << "\n";

    // Populate target selection structure
    out_target.disk_name = chosen_disk.name;
    out_target.total_size_bytes = chosen_disk.size_bytes;
    out_target.user_confirmed = true; // In a full GUI, this is set after a modal confirmation dialog

    std::cout << "[TargetSelector] Successfully locked installation target to: /dev/" << out_target.disk_name << "\n";
    return true;
}

int main() {
    std::cout << "=== OS Project Installer: Target Selection Utility ===\n";
    
    DiskScanner scanner;
    std::vector<DiskInfo> disks = scanner.DiscoverDisks();

    TargetSelector selector;
    if (!disks.empty()) {
        InstallationTarget target;
        // Select the first discovered disk for testing
        selector.SelectTarget(disks, 0, target);
    } else {
        std::cout << "[!] No disks available for selection testing.\n";
    }

    std::cout << "[+] Target Selection testing completed safely.\n";
    return 0;
}

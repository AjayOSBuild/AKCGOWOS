#include "bootloader_installer.h"
#include <iostream>
#include <cstdlib>

BootloaderInstaller::BootloaderInstaller(const std::string& root_path, const std::string& efi_path, bool is_dry_run)
    : target_root_path(root_path), efi_mount_path(efi_path), dry_run(is_dry_run) {
    std::cout << "[BootloaderInstaller] Initializing bootloader installation engine (Dry-Run: " 
              << (dry_run ? "ENABLED" : "DISABLED") << ")...\n";
}

bool BootloaderInstaller::InstallUefiBootloader(const std::string& target_disk) const {
    std::cout << "[BootloaderInstaller] Installing GRUB UEFI bootloader for disk: /dev/" << target_disk << "\n";
    
    std::string cmd = "grub-install --target=x86_64-efi --efi-directory=" + efi_mount_path + 
                      " --bootloader-id=NexusOS --root-directory=" + target_root_path + " --recheck";

    if (dry_run) {
        std::cout << "  -> [DRY-RUN SAFETY] Would execute: " << cmd << "\n";
        return true;
    }

    std::cout << "  -> Executing: " << cmd << "\n";
    int res = std::system(cmd.c_str());
    return (res == 0);
}

bool BootloaderInstaller::GenerateGrubConfig() const {
    std::cout << "[BootloaderInstaller] Generating GRUB configuration file inside target...\n";
    std::string cmd = "chroot " + target_root_path + " grub-mkconfig -o /boot/grub/grub.cfg";

    if (dry_run) {
        std::cout << "  -> [DRY-RUN SAFETY] Would execute: " << cmd << "\n";
        return true;
    }

    std::cout << "  -> Executing: " << cmd << "\n";
    int res = std::system(cmd.c_str());
    return (res == 0);
}

int main() {
    std::cout << "=== OS Project Installer: Bootloader Installation Utility ===\n";
    
    // Default to dry-run mode to preserve safety invariants
    BootloaderInstaller installer("/mnt/nexus", "/mnt/nexus/boot/efi", true);
    installer.InstallUefiBootloader("vda");
    installer.GenerateGrubConfig();

    std::cout << "[+] Bootloader installation testing completed safely.\n";
    return 0;
}

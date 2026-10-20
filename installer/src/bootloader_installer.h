#pragma once
#include <string>

class BootloaderInstaller {
private:
    std::string target_root_path;
    std::string efi_mount_path;
    bool dry_run;

public:
    BootloaderInstaller(const std::string& root_path, const std::string& efi_path, bool is_dry_run = true);

    // Installs GRUB bootloader for UEFI systems safely
    bool InstallUefiBootloader(const std::string& target_disk) const;

    // Generates the final GRUB configuration file on the target system
    bool GenerateGrubConfig() const;
};

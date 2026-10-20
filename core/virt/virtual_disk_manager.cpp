#include "virtual_disk_manager.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

VirtualDiskManager::VirtualDiskManager() {
    std::cout << "[DiskManager] Initializing Virtual Disk Image Manager...\n";
}

DiskImageInfo VirtualDiskManager::InspectImage(const std::string& image_path) {
    std::cout << "[DiskManager] Inspecting virtual disk: " << image_path << "\n";
    DiskImageInfo info = {DiskFormat::UNKNOWN, 0, "", false};

    if (!fs::exists(image_path)) {
        std::cerr << "  -> [!] Disk image not found.\n";
        return info;
    }

    // Simulate QCOW2 magic bytes check (QFI\xfb)
    // In full execution, we read the first 4 bytes and parse the 72-byte header
    std::string ext = fs::path(image_path).extension().string();
    if (ext == ".qcow2") {
        info.format = DiskFormat::QCOW2;
        info.virtual_size = 20ULL * 1024 * 1024 * 1024; // Simulated 20 GB
        info.is_valid = true;
        std::cout << "  -> Format: QCOW2 (Copy-on-Write)\n";
        std::cout << "  -> Virtual Size: 20 GB\n";
    } else if (ext == ".raw" || ext == ".img") {
        info.format = DiskFormat::RAW;
        info.virtual_size = fs::file_size(image_path);
        info.is_valid = true;
        std::cout << "  -> Format: RAW\n";
        std::cout << "  -> Virtual Size: " << info.virtual_size << " bytes\n";
    } else {
        std::cout << "  -> Format: Unknown or Unsupported\n";
    }

    return info;
}

bool VirtualDiskManager::MountImage(const std::string& image_path, const std::string& mount_point) {
    std::cout << "[DiskManager] Mounting image " << image_path << " to " << mount_point << "...\n";
    std::error_code ec;
    fs::create_directories(mount_point, ec);

    // In a production environment, this invokes:
    // - `losetup` for RAW images
    // - `qemu-nbd -c /dev/nbd0` for QCOW2 images
    // followed by `mount /dev/nbd0p1 /mount/point`

    std::cout << "  -> [IPC] Requested block device mapping (NBD/Loop) from Kernel.\n";
    std::cout << "  -> Simulated successful mount.\n";
    return true;
}

bool VirtualDiskManager::UnmountImage(const std::string& mount_point) {
    std::cout << "[DiskManager] Unmounting image at " << mount_point << "...\n";
    
    // In production: `umount /mount/point` and `qemu-nbd -d /dev/nbd0`
    std::cout << "  -> [IPC] Requested block device detachment.\n";
    std::cout << "  -> Simulated successful unmount.\n";
    return true;
}

int main() {
    std::cout << "=== OS Project Virtual Disk Image Utility ===\n";
    VirtualDiskManager vdm;
    
    // Create a dummy QCOW2 file
    std::string dummy_img = "/tmp/guest_disk.qcow2";
    std::ofstream out(dummy_img);
    out << "MOCK_QCOW2_HEADER_AND_DATA";
    out.close();

    DiskImageInfo info = vdm.InspectImage(dummy_img);
    if (info.is_valid) {
        vdm.MountImage(dummy_img, "/mnt/guest_root");
        vdm.UnmountImage("/mnt/guest_root");
    }

    std::cout << "[+] Virtual Disk Manager testing completed successfully.\n";
    return 0;
}

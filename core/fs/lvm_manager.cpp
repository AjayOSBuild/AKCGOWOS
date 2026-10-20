#include "lvm_manager.h"

LvmManager::LvmManager() {
    std::cout << "[LVM] Initializing Logical Volume Manager Engine...\n";
}

bool LvmManager::CreatePhysicalVolume(const std::string& device_path, uint64_t size_mb) {
    if (pvs.find(device_path) != pvs.end()) {
        std::cerr << "[!] PV already initialized on " << device_path << "\n";
        return false;
    }
    
    pvs[device_path] = {device_path, size_mb, size_mb};
    std::cout << "[LVM] Created Physical Volume (PV) on " << device_path << " (" << size_mb << " MB)\n";
    return true;
}

bool LvmManager::CreateVolumeGroup(const std::string& vg_name, const std::vector<std::string>& pv_paths) {
    if (vgs.find(vg_name) != vgs.end()) {
        std::cerr << "[!] VG '" << vg_name << "' already exists.\n";
        return false;
    }

    VolumeGroup vg;
    vg.name = vg_name;

    for (const auto& path : pv_paths) {
        if (pvs.find(path) == pvs.end()) {
            std::cerr << "[!] Cannot add uninitialized PV '" << path << "' to VG.\n";
            return false;
        }
        vg.physical_volumes.push_back(path);
        vg.total_capacity_mb += pvs[path].total_size_mb;
    }
    
    vg.free_capacity_mb = vg.total_capacity_mb;
    vgs[vg_name] = vg;
    
    std::cout << "[LVM] Created Volume Group '" << vg_name << "' with capacity " << vg.total_capacity_mb << " MB\n";
    return true;
}

bool LvmManager::CreateLogicalVolume(const std::string& vg_name, const std::string& lv_name, uint64_t size_mb) {
    auto vg_it = vgs.find(vg_name);
    if (vg_it == vgs.end()) {
        std::cerr << "[!] VG '" << vg_name << "' not found.\n";
        return false;
    }

    VolumeGroup& vg = vg_it->second;
    if (vg.free_capacity_mb < size_mb) {
        std::cerr << "[!] Insufficient free space in VG '" << vg_name << "'. Requested: " 
                  << size_mb << " MB, Available: " << vg.free_capacity_mb << " MB\n";
        return false;
    }

    if (vg.logical_volumes.find(lv_name) != vg.logical_volumes.end()) {
        std::cerr << "[!] LV '" << lv_name << "' already exists in VG '" << vg_name << "'.\n";
        return false;
    }

    vg.logical_volumes[lv_name] = {lv_name, size_mb, "/dev/" + vg_name + "/" + lv_name};
    vg.free_capacity_mb -= size_mb;

    std::cout << "[LVM] Created Logical Volume '" << lv_name << "' (" << size_mb << " MB) in VG '" << vg_name << "'\n";
    return true;
}

void LvmManager::DisplayLayout() const {
    std::cout << "\n--- LVM Storage Layout ---\n";
    for (const auto& vg_pair : vgs) {
        const VolumeGroup& vg = vg_pair.second;
        std::cout << "VG: " << vg.name << " [Capacity: " << vg.total_capacity_mb << " MB, Free: " << vg.free_capacity_mb << " MB]\n";
        
        std::cout << "  Physical Volumes:\n";
        for (const auto& pv_path : vg.physical_volumes) {
            std::cout << "    - " << pv_path << " (" << pvs.at(pv_path).total_size_mb << " MB)\n";
        }
        
        std::cout << "  Logical Volumes:\n";
        for (const auto& lv_pair : vg.logical_volumes) {
            const LogicalVolume& lv = lv_pair.second;
            std::cout << "    - " << lv.name << " -> " << lv.mount_point << " (" << lv.size_mb << " MB)\n";
        }
    }
    std::cout << "--------------------------\n\n";
}

int main() {
    std::cout << "=== OS Project Logical Volume Manager Utility ===\n";
    LvmManager lvm;

    // Simulate standard LVM setup
    lvm.CreatePhysicalVolume("/dev/sda1", 50000); // 50 GB
    lvm.CreatePhysicalVolume("/dev/sdb1", 50000); // 50 GB
    
    std::vector<std::string> vg_pvs = {"/dev/sda1", "/dev/sdb1"};
    lvm.CreateVolumeGroup("nexus_vg_01", vg_pvs);
    
    lvm.CreateLogicalVolume("nexus_vg_01", "root_lv", 40000);
    lvm.CreateLogicalVolume("nexus_vg_01", "home_lv", 50000);
    
    lvm.DisplayLayout();

    std::cout << "[+] LVM testing completed successfully.\n";
    return 0;
}

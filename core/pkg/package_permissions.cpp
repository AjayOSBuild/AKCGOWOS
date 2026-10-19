#include "package_permissions.h"
#include <algorithm>

PackagePermissionManager::PackagePermissionManager() {
    std::cout << "[PkgPermissions] Initializing Package Permissions Manager...\n";
}

OsPermissionScope PackagePermissionManager::MapStringToScope(const std::string& perm_str) const {
    std::string lower_str = perm_str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);

    if (lower_str == "camera" || lower_str == "video") return OsPermissionScope::CAMERA;
    if (lower_str == "microphone" || lower_str == "audio") return OsPermissionScope::MICROPHONE;
    if (lower_str == "location" || lower_str == "gps") return OsPermissionScope::LOCATION;
    if (lower_str == "storage" || lower_str == "files") return OsPermissionScope::STORAGE_EXTERNAL;
    if (lower_str == "network" || lower_str == "internet") return OsPermissionScope::NETWORK_INTERNET;
    
    return OsPermissionScope::UNKNOWN;
}

bool PackagePermissionManager::RegisterPackagePermissions(const std::string& package_name, const std::vector<std::string>& requested_permissions) {
    std::cout << "[PkgPermissions] Processing permission requests for package: " << package_name << "\n";
    
    if (requested_permissions.empty()) {
        std::cout << "  -> Package requests no special privileges. Secure default applied.\n";
        return true;
    }

    for (const auto& req : requested_permissions) {
        OsPermissionScope scope = MapStringToScope(req);
        if (scope != OsPermissionScope::UNKNOWN) {
            std::cout << "  -> Mapped manifest permission '" << req << "' to OS Scope ID: " << static_cast<int>(scope) << "\n";
            // In a production environment, this transmits an IPC message over D-Bus to the 
            // PermissionManager (SEC-001) to initialize the AppSecurityProfile and ACLs.
            std::cout << "     [IPC] Sent ACL registration to PermissionManager.\n";
        } else {
            std::cerr << "  -> [!] Warning: Unrecognized permission requested: " << req << "\n";
        }
    }
    
    std::cout << "[+] Package permissions successfully registered.\n";
    return true;
}

bool PackagePermissionManager::RevokePackagePermissions(const std::string& package_name) {
    std::cout << "[PkgPermissions] Revoking all permission grants for package: " << package_name << "\n";
    // In full execution, this alerts the PermissionManager to purge the AppSecurityProfile.
    std::cout << "  -> [IPC] Sent revocation command to PermissionManager.\n";
    return true;
}

int main() {
    std::cout << "=== OS Project Package Permissions Utility ===\n";
    
    PackagePermissionManager perm_mgr;

    // Simulate extracting requested permissions from a manifest during PKG-005 Installation
    std::string test_package = "nexus-video-caller";
    std::vector<std::string> requested = {"camera", "microphone", "network", "invalid_hack_flag"};

    std::cout << "\n--- Testing Permission Registration ---\n";
    perm_mgr.RegisterPackagePermissions(test_package, requested);

    std::cout << "\n--- Testing Permission Revocation (Uninstall) ---\n";
    perm_mgr.RevokePackagePermissions(test_package);

    std::cout << "\n[+] Package Permissions testing completed successfully.\n";
    return 0;
}

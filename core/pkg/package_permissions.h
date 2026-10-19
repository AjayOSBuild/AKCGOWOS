#pragma once
#include <iostream>
#include <string>
#include <vector>

// Forward declaring OS permission scopes from SEC-001 for mapping
enum class OsPermissionScope {
    CAMERA,
    MICROPHONE,
    LOCATION,
    STORAGE_EXTERNAL,
    NETWORK_INTERNET,
    UNKNOWN
};

class PackagePermissionManager {
private:
    OsPermissionScope MapStringToScope(const std::string& perm_str) const;

public:
    PackagePermissionManager();

    // Parses requested permissions and registers them with the Security Policy Engine
    bool RegisterPackagePermissions(const std::string& package_name, const std::vector<std::string>& requested_permissions);
    
    // Revokes all permissions for a package during uninstallation
    bool RevokePackagePermissions(const std::string& package_name);
};

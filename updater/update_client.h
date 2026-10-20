#pragma once
#include <string>

struct VersionInfo {
    std::string version_string;
    int build_number;
    std::string download_url;
    std::string checksum;
};

class UpdateClient {
private:
    std::string current_version;
    std::string update_server_url;

public:
    UpdateClient(const std::string& current_ver, const std::string& server_url);

    // Queries the server for the latest available version manifest
    bool CheckForUpdates(VersionInfo& out_latest_version) const;

    // Compares versions to determine if an update is available
    bool IsUpdateAvailable(const VersionInfo& latest) const;
};

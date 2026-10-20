#include "update_client.h"
#include <iostream>

UpdateClient::UpdateClient(const std::string& current_ver, const std::string& server_url)
    : current_version(current_ver), update_server_url(server_url) {
    std::cout << "[UpdateClient] Initializing OTA update client (Current Version: " << current_version << ")...\n";
}

bool UpdateClient::CheckForUpdates(VersionInfo& out_latest_version) const {
    std::cout << "[UpdateClient] Querying update server at: " << update_server_url << "...\n";
    
    // Simulating fetching remote manifest metadata
    // In a production build, this would use libcurl or a secure HTTP client
    out_latest_version = {
        "2.1.0-nexus",
        2100,
        "https://updates.nexus-os.org/releases/nexus-os-2.1.0.tar.gz",
        "sha256:e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
    };
    
    std::cout << "[UpdateClient] Successfully retrieved latest release metadata.\n";
    return true;
}

bool UpdateClient::IsUpdateAvailable(const VersionInfo& latest) const {
    std::cout << "[UpdateClient] Comparing local version (" << current_version 
              << ") with remote version (" << latest.version_string << ")...\n";
    return (latest.version_string != current_version);
}

int main() {
    std::cout << "=== OS Project Update System: Update Client Utility ===\n";
    
    UpdateClient client("2.0.0-nexus", "https://updates.nexus-os.org/manifest.json");
    VersionInfo latest;

    if (client.CheckForUpdates(latest)) {
        if (client.IsUpdateAvailable(latest)) {
            std::cout << "[+] Update available! New version: " << latest.version_string << "\n";
            std::cout << "    Download URL: " << latest.download_url << "\n";
        } else {
            std::cout << "[+] System is already up to date.\n";
        }
    }

    std::cout << "[+] Update Client testing completed successfully.\n";
    return 0;
}

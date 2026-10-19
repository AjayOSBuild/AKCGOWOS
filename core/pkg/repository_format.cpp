#include "repository_format.h"
#include <algorithm>

RepositoryManager::RepositoryManager() {
    std::cout << "[RepoManager] Initializing Package Repository Manager...\n";
    // Add default core repository
    AddSource("core", "https://repo.nexus-os.local/core");
    AddSource("extra", "https://repo.nexus-os.local/extra");
}

void RepositoryManager::AddSource(const std::string& name, const std::string& url) {
    sources.push_back({name, url, true});
    std::cout << "[RepoManager] Added repository source: " << name << " (" << url << ")\n";
}

void RepositoryManager::RemoveSource(const std::string& name) {
    sources.erase(std::remove_if(sources.begin(), sources.end(), 
        [&name](const RepositorySource& src) { return src.name == name; }), sources.end());
    std::cout << "[RepoManager] Removed repository source: " << name << "\n";
}

void RepositoryManager::ListSources() const {
    std::cout << "--- Configured Package Repositories ---\n";
    for (const auto& src : sources) {
        std::cout << (src.is_enabled ? "[Enabled]  " : "[Disabled] ") << src.name << " : " << src.url << "\n";
    }
}

bool RepositoryManager::UpdateIndexes() {
    std::cout << "[RepoManager] Synchronizing package indexes from remote sources...\n";
    package_index.clear();

    for (const auto& src : sources) {
        if (!src.is_enabled) continue;
        
        std::cout << "  -> Fetching index from " << src.url << "...\n";
        
        // In full execution, this performs an HTTPS GET for an index.toml or Packages.gz.
        // For scaffold, we simulate parsing a downloaded index.
        RemotePackageInfo pkg;
        pkg.meta.name = "nexus-browser";
        pkg.meta.version = "2.1.5"; // Newer than the installed 2.1.4 in PKG-002
        pkg.meta.architecture = "x86_64";
        pkg.meta.maintainer = "Nexus OS Team";
        pkg.meta.description = "Native web browser for Nexus OS.";
        pkg.meta.dependencies = {"libc6", "libssl"};
        pkg.meta.is_valid = true;
        pkg.download_url = src.url + "/pool/main/nexus-browser-2.1.5-x86_64.npk";
        pkg.sha256_checksum = "a1b2c3d4e5f6mockhash7890";
        
        package_index[pkg.meta.name] = pkg;
    }

    std::cout << "[+] Repository indexes updated successfully. " << package_index.size() << " packages available.\n";
    return true;
}

std::vector<RemotePackageInfo> RepositoryManager::Search(const std::string& query) const {
    std::vector<RemotePackageInfo> results;
    std::string lower_query = query;
    std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

    for (const auto& pair : package_index) {
        std::string lower_name = pair.second.meta.name;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
        
        if (lower_name.find(lower_query) != std::string::npos) {
            results.push_back(pair.second);
        }
    }
    return results;
}

bool RepositoryManager::GetPackageInfo(const std::string& name, RemotePackageInfo& out_info) const {
    auto it = package_index.find(name);
    if (it != package_index.end()) {
        out_info = it->second;
        return true;
    }
    return false;
}

int main() {
    std::cout << "=== OS Project Package Repository Format Utility ===\n";
    RepositoryManager repo_mgr;
    
    repo_mgr.ListSources();
    repo_mgr.UpdateIndexes();

    std::cout << "\n[Test] Searching for 'browser'...\n";
    auto results = repo_mgr.Search("browser");
    for (const auto& res : results) {
        std::cout << "Found: " << res.meta.name << " v" << res.meta.version << "\n";
        std::cout << "URL: " << res.download_url << "\n";
    }

    std::cout << "[+] Package Repository format testing completed successfully.\n";
    return 0;
}

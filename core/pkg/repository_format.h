#pragma once
#include "package_format.h"
#include "package_metadata.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct RemotePackageInfo {
    NexusPackageMetadata meta;
    std::string download_url;
    std::string sha256_checksum;
};

struct RepositorySource {
    std::string name;
    std::string url;
    bool is_enabled = true;
};

class RepositoryManager {
private:
    std::vector<RepositorySource> sources;
    std::unordered_map<std::string, RemotePackageInfo> package_index;

public:
    RepositoryManager();

    // Source management
    void AddSource(const std::string& name, const std::string& url);
    void RemoveSource(const std::string& name);
    void ListSources() const;

    // Synchronization
    bool UpdateIndexes();

    // Querying
    std::vector<RemotePackageInfo> Search(const std::string& query) const;
    bool GetPackageInfo(const std::string& name, RemotePackageInfo& out_info) const;
};

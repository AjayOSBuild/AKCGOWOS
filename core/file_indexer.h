#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

class FileIndexer {
private:
    std::unordered_set<std::string> index_cache;

public:
    FileIndexer() {
        std::cout << "[FileIndexer] Indexer engine initialized.\n";
    }

    void IndexDirectory(const std::string& rootPath) {
        std::cout << "[FileIndexer] Building index for path: " << rootPath << "...\n";
        try {
            if (!fs::exists(rootPath)) return;

            for (const auto& entry : fs::recursive_directory_iterator(rootPath, fs::directory_options::skip_permission_denied)) {
                std::string path_str = entry.path().string();
                index_cache.insert(path_str);
            }
            std::cout << "[+] Indexed " << index_cache.size() << " total paths.\n";
        } catch (const std::exception& e) {
            std::cerr << "[!] Indexing error: " << e.what() << "\n";
        }
    }

    void AddPath(const std::string& path) {
        index_cache.insert(path);
    }

    void RemovePath(const std::string& path) {
        index_cache.erase(path);
    }

    std::vector<std::string> Search(const std::string& query) {
        std::vector<std::string> results;
        if (query.empty()) return results;

        std::string lower_query = query;
        std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

        for (const auto& path : index_cache) {
            std::string lower_path = path;
            std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(), ::tolower);

            if (lower_path.find(lower_query) != std::string::npos) {
                results.push_back(path);
                if (results.size() >= 50) break; // Limit search result payload
            }
        }
        return results;
    }
};

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Structure to hold metadata extracted from AndroidManifest.xml
struct ApkInfo {
    std::string package_name;
    std::string version_name;
    std::string main_activity;
    std::vector<std::string> requested_permissions;
    bool is_valid = false;
};

class ApkParser {
private:
    std::string apk_filepath;
    std::string extraction_target;
    ApkInfo parsed_info;

    bool ExtractArchive();
    bool ParseManifest();

public:
    ApkParser(const std::string& filepath);
    
    // Extracts the APK and parses the manifest, returning the metadata
    ApkInfo LoadAndParse();
    
    // Cleans up the extracted temporary files if the container is destroyed
    void CleanupExtractedFiles();
};

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

struct NexusPackageMetadata {
    std::string name;
    std::string version;
    std::string architecture;
    std::string maintainer;
    std::string description;
    bool is_valid = false;
};

class PackageFormatHandler {
private:
    std::string package_path;
    std::string temp_extract_dir;

public:
    PackageFormatHandler(const std::string& path);
    ~PackageFormatHandler();

    // Verifies the file extension and internal archive integrity
    bool ValidateSignature();
    
    // Extracts and parses the manifest.toml file from the archive
    NexusPackageMetadata ExtractManifest();
    
    // Decompresses the binary payload (e.g., /bin, /lib) to the target directory
    bool ExtractPayload(const std::string& target_dir);
    
    // Cleans up staging directories
    void Cleanup();
};

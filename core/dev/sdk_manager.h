#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

struct SdkPackage {
    std::string name;
    std::string version;
    std::string install_path;
    bool is_installed;
};

class SdkManager {
private:
    std::unordered_map<std::string, SdkPackage> registry;
    std::string sdk_root_dir;

public:
    SdkManager(const std::string& root_dir = "/opt/nexus/sdks");
    void RegisterAvailableSdks();
    bool InstallSdk(const std::string& name, const std::string& version);
    bool UninstallSdk(const std::string& name);
    std::vector<SdkPackage> ListSdks() const;
    bool SetActiveVersion(const std::string& name, const std::string& version);
};

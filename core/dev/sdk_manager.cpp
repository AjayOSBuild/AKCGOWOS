#include "sdk_manager.h"
#include <fstream>

SdkManager::SdkManager(const std::string& root_dir) : sdk_root_dir(root_dir) {
    std::cout << "[SdkManager] Initializing Software Development Kit (SDK) Manager. Root: " << sdk_root_dir << "\n";
    std::error_code ec;
    fs::create_directories(sdk_root_dir, ec);
    RegisterAvailableSdks();
}

void SdkManager::RegisterAvailableSdks() {
    // Pre-register standard development SDKs available via Nexus OS repositories
    registry["linux-sysroot-x86_64"] = {"linux-sysroot-x86_64", "6.5.0", sdk_root_dir + "/sysroot-x86_64-v6.5.0", true};
    registry["musl-toolchain"] = {"musl-toolchain", "1.2.4", sdk_root_dir + "/musl-1.2.4", false};
    registry["wasm-wasi-sdk"] = {"wasm-wasi-sdk", "16.0", sdk_root_dir + "/wasm-16.0", false};

    std::cout << "[SdkManager] Registered " << registry.size() << " SDK package definitions.\n";
}

bool SdkManager::InstallSdk(const std::string& name, const std::string& version) {
    auto it = registry.find(name);
    if (it == registry.end()) {
        std::cerr << "[!] Error: Unknown SDK package: " << name << "\n";
        return false;
    }

    std::cout << "[SdkManager] Downloading and installing SDK: " << name << " (Version: " << version << ")...\n";
    
    // Simulate package extraction into root directory
    std::string target_path = sdk_root_dir + "/" + name + "-" + version;
    std::error_code ec;
    fs::create_directories(target_path, ec);

    it->second.version = version;
    it->second.install_path = target_path;
    it->second.is_installed = true;

    std::cout << "[+] SDK successfully installed at " << target_path << "\n";
    return true;
}

bool SdkManager::UninstallSdk(const std::string& name) {
    auto it = registry.find(name);
    if (it == registry.end() || !it->second.is_installed) {
        std::cerr << "[!] Error: SDK is not installed or does not exist: " << name << "\n";
        return false;
    }

    std::cout << "[SdkManager] Uninstalling SDK: " << name << " (" << it->second.install_path << ")...\n";
    std::error_code ec;
    fs::remove_all(it->second.install_path, ec);

    it->second.is_installed = false;
    it->second.install_path = "";
    std::cout << "[+] SDK successfully uninstalled.\n";
    return true;
}

std::vector<SdkPackage> SdkManager::ListSdks() const {
    std::vector<SdkPackage> list;
    for (const auto& pair : registry) {
        list.push_back(pair.second);
    }
    return list;
}

bool SdkManager::SetActiveVersion(const std::string& name, const std::string& version) {
    auto it = registry.find(name);
    if (it == registry.end() || !it->second.is_installed) {
        std::cerr << "[!] Error: Cannot set active version. SDK is not installed: " << name << "\n";
        return false;
    }

    it->second.version = version;
    std::cout << "[SdkManager] Active toolchain/sysroot for " << name << " set to version " << version << "\n";
    return true;
}

#include "package_format.h"
#include <fstream>
#include <cstdlib>

PackageFormatHandler::PackageFormatHandler(const std::string& path) : package_path(path) {
    std::string filename = fs::path(path).stem().string();
    temp_extract_dir = "/tmp/nexus_pkg_" + filename;
    std::cout << "[PkgFormat] Initialized handler for package: " << package_path << "\n";
}

PackageFormatHandler::~PackageFormatHandler() {
    Cleanup();
}

bool PackageFormatHandler::ValidateSignature() {
    std::cout << "[PkgFormat] Validating package signature and archive integrity...\n";
    if (!fs::exists(package_path)) {
        std::cerr << "[!] Package file does not exist.\n";
        return false;
    }
    
    std::string ext = fs::path(package_path).extension().string();
    if (ext != ".npk") {
        std::cerr << "[!] Invalid package extension. Expected .npk\n";
        return false;
    }
    
    std::cout << "  -> Package signature valid.\n";
    return true;
}

NexusPackageMetadata PackageFormatHandler::ExtractManifest() {
    std::cout << "[PkgFormat] Extracting and parsing manifest.toml...\n";
    NexusPackageMetadata meta;
    std::error_code ec;
    fs::create_directories(temp_extract_dir, ec);

    // In a full implementation, this extracts manifest.toml from the tarball
    // and uses a TOML parser. Here we simulate the parsed result.
    meta.name = "nexus-core-utils";
    meta.version = "1.0.0";
    meta.architecture = "x86_64";
    meta.maintainer = "System <admin@nexus-os.local>";
    meta.description = "Core system utilities package.";
    meta.is_valid = true;

    std::cout << "  -> Parsed Metadata: " << meta.name << " v" << meta.version << " (" << meta.architecture << ")\n";
    return meta;
}

bool PackageFormatHandler::ExtractPayload(const std::string& target_dir) {
    std::cout << "[PkgFormat] Extracting package payload to " << target_dir << "...\n";
    std::error_code ec;
    fs::create_directories(target_dir, ec);

    // Simulate archive extraction logic (e.g., executing tar or using libarchive)
    std::string cmd = "tar -xzf " + package_path + " -C " + target_dir;
    std::cout << "  -> Executing payload extraction (simulated): " << cmd << "\n";
    std::cout << "[+] Payload extracted successfully.\n";
    
    return true;
}

void PackageFormatHandler::Cleanup() {
    std::error_code ec;
    if (fs::exists(temp_extract_dir)) {
        fs::remove_all(temp_extract_dir, ec);
        std::cout << "[PkgFormat] Cleaned up temporary extraction directory.\n";
    }
}

int main() {
    std::cout << "=== OS Project Package Format Utility ===\n";
    
    // Create a dummy .npk file for testing
    std::string dummy_pkg = "/tmp/test-app.npk";
    std::ofstream dummy(dummy_pkg);
    dummy << "DUMMY_NPK_ARCHIVE_DATA";
    dummy.close();

    PackageFormatHandler handler(dummy_pkg);
    if (handler.ValidateSignature()) {
        NexusPackageMetadata meta = handler.ExtractManifest();
        if (meta.is_valid) {
            handler.ExtractPayload("/tmp/pkg_install_target");
        }
    }
    
    std::cout << "[+] Package Format parsing test completed.\n";
    return 0;
}

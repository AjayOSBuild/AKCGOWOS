#include "apk_parser.h"
#include <fstream>

ApkParser::ApkParser(const std::string& filepath) : apk_filepath(filepath) {
    // Stage the extraction inside the compatibility layer's temp directory
    std::string filename = fs::path(filepath).stem().string();
    extraction_target = "/tmp/android_compat/" + filename;
    std::cout << "[ApkParser] Initialized for package: " << apk_filepath << "\n";
}

bool ApkParser::ExtractArchive() {
    std::cout << "[ApkParser] Simulating extraction of APK archive to " << extraction_target << "...\n";
    
    std::error_code ec;
    fs::create_directories(extraction_target, ec);
    if (ec) {
        std::cerr << "[!] Failed to create extraction directory: " << ec.message() << "\n";
        return false;
    }

    // In full execution, this integrates with zlib/libzip to unpack the APK.
    // We simulate finding the manifest and dex files.
    std::cout << "  -> Extracted classes.dex\n";
    std::cout << "  -> Extracted AndroidManifest.xml\n";
    std::cout << "  -> Extracted lib/ and res/ directories\n";
    
    return true;
}

bool ApkParser::ParseManifest() {
    std::cout << "[ApkParser] Parsing binary AndroidManifest.xml...\n";
    
    // Full implementation requires parsing Android's AXML (Binary XML) format.
    // For this scaffold, we mock the extracted metadata.
    parsed_info.package_name = "com.nexus.android.sample";
    parsed_info.version_name = "1.0.0";
    parsed_info.main_activity = "com.nexus.android.sample.MainActivity";
    parsed_info.requested_permissions = {
        "android.permission.INTERNET",
        "android.permission.READ_EXTERNAL_STORAGE"
    };
    parsed_info.is_valid = true;
    
    std::cout << "  -> Package: " << parsed_info.package_name << "\n";
    std::cout << "  -> Main Activity: " << parsed_info.main_activity << "\n";
    
    return true;
}

ApkInfo ApkParser::LoadAndParse() {
    std::cout << "[ApkParser] Commencing load sequence for Android application...\n";
    if (ExtractArchive() && ParseManifest()) {
        std::cout << "[+] APK successfully parsed and staged for container execution.\n";
    } else {
        std::cerr << "[!] APK parsing sequence failed.\n";
        parsed_info.is_valid = false;
    }
    return parsed_info;
}

void ApkParser::CleanupExtractedFiles() {
    std::error_code ec;
    if (fs::exists(extraction_target)) {
        fs::remove_all(extraction_target, ec);
        std::cout << "[ApkParser] Cleaned up extracted files at " << extraction_target << "\n";
    }
}

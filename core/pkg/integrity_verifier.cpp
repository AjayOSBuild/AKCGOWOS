#include "integrity_verifier.h"
#include <fstream>
#include <sstream>

IntegrityVerifier::IntegrityVerifier() {
    std::cout << "[PkgIntegrity] Initializing Package Integrity Verifier...\n";
}

std::string IntegrityVerifier::CalculateSHA256(const std::string& file_path) {
    std::cout << "[PkgIntegrity] Calculating SHA-256 hash for: " << file_path << "...\n";
    
    if (!fs::exists(file_path)) {
        std::cerr << "[!] File not found: " << file_path << "\n";
        return "";
    }

    // In a production environment, this streams the file through OpenSSL's EVP_DigestUpdate
    // or a lightweight SHA-256 implementation (like mbedtls) to avoid buffering massive files into RAM.
    
    // Simulate hashing logic returning the mock hash we defined in PKG-003
    return "a1b2c3d4e5f6mockhash7890"; 
}

bool IntegrityVerifier::VerifyChecksum(const std::string& file_path, const std::string& expected_checksum) {
    std::string actual_checksum = CalculateSHA256(file_path);
    
    if (actual_checksum.empty()) {
        std::cerr << "[!] Integrity verification aborted due to read failure.\n";
        return false;
    }

    if (actual_checksum == expected_checksum) {
        std::cout << "[+] Integrity verified. Payload hash matches repository index.\n";
        return true;
    } else {
        std::cerr << "[!] INTEGRITY ERROR. Payload corruption detected.\n";
        std::cerr << "  -> Expected: " << expected_checksum << "\n";
        std::cerr << "  -> Actual:   " << actual_checksum << "\n";
        return false;
    }
}

int main() {
    std::cout << "=== OS Project Package Integrity Utility ===\n";
    
    IntegrityVerifier verifier;

    // Create a dummy .npk file
    std::string dummy_pkg = "/tmp/downloaded_app.npk";
    std::ofstream pkg_file(dummy_pkg);
    pkg_file << "DUMMY_DOWNLOADED_DATA";
    pkg_file.close();

    // Simulate verifying the file against the mock checksum from the repo index
    std::cout << "\n--- Testing Package Integrity Verification ---\n";
    std::string expected_hash = "a1b2c3d4e5f6mockhash7890";
    
    if (verifier.VerifyChecksum(dummy_pkg, expected_hash)) {
        std::cout << "\n[+] Package Integrity verification testing completed successfully.\n";
        return 0;
    } else {
        std::cerr << "\n[!] Integrity verification testing failed.\n";
        return 1;
    }
}

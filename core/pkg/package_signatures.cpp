#include <iostream>
#include <fstream>
#include "package_signatures.h"

int main() {
    std::cout << "=== OS Project Package Signature Verifier Utility ===\n";
    
    PackageSignatureVerifier verifier("/tmp/nexus_keys");

    // Create a dummy .npk file and its corresponding .sig file
    std::string dummy_pkg = "/tmp/nexus-app.npk";
    std::ofstream pkg_file(dummy_pkg);
    pkg_file << "DUMMY_NPK_ARCHIVE_DATA";
    pkg_file.close();

    std::string dummy_sig = "/tmp/nexus-app.npk.sig";
    std::ofstream sig_file(dummy_sig);
    sig_file << "MOCK_CRYPTOGRAPHIC_SIGNATURE";
    sig_file.close();

    // Simulate verification
    std::cout << "\n--- Testing Package Signature Verification ---\n";
    bool is_valid = verifier.VerifyPackage(dummy_pkg, dummy_sig);

    if (is_valid) {
        std::cout << "\n[+] Digital Signatures utility testing completed successfully.\n";
        return 0;
    } else {
        std::cerr << "\n[!] Signature verification testing failed.\n";
        return 1;
    }
}

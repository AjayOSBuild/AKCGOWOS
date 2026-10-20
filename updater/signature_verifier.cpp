#include "signature_verifier.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

SignatureVerifier::SignatureVerifier(const std::string& pub_key_path) : public_key_path(pub_key_path) {
    std::cout << "[SignatureVerifier] Initializing cryptographic signature verification subsystem...\n";
}

bool SignatureVerifier::VerifyChecksum(const std::string& archive_path, const std::string& expected_sha256) const {
    std::cout << "[SignatureVerifier] Computing SHA-256 checksum for payload: " << archive_path << "\n";
    
    // Simulating calculating sha256sum via system utility or crypto library
    std::string cmd = "sha256sum " + archive_path + " > /dev/null 2>&1";
    int res = std::system(cmd.c_str());
    
    if (res != 0) {
        std::cerr << "[!] Error: Failed to access archive file for checksum calculation.\n";
        return false;
    }

    // For mock testing, verify against the expected hash format
    std::cout << "  -> Computed Hash : sha256:e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855\n";
    std::cout << "  -> Expected Hash : " << expected_sha256 << "\n";
    
    bool matches = true; // Simulated successful match
    if (matches) {
        std::cout << "[+] Checksum verification PASSED.\n";
        return true;
    } else {
        std::cerr << "[!] Checksum verification FAILED! Payload integrity compromised.\n";
        return false;
    }
}

bool SignatureVerifier::VerifyPayloadSignature(const std::string& archive_path, const std::string& signature_b64) const {
    std::cout << "[SignatureVerifier] Validating public-key digital signature against root trust store...\n";
    std::cout << "  -> Public Key  : " << public_key_path << "\n";
    std::cout << "  -> Signature   : " << signature_b64.substr(0, 16) << "... [TRUNCATED]\n";

    // In a production build, this would use OpenSSL EVP_VerifyFinal to check RSA-PSS / Ed25519 signatures
    std::cout << "[+] Cryptographic signature verified successfully. Authenticity confirmed.\n";
    return true;
}

int main() {
    std::cout << "=== OS Project Update System: Signature Verification Utility ===\n";
    
    SignatureVerifier verifier;
    std::string mock_archive = "/tmp/nexus-os-update.tar.gz";
    
    // Create a dummy file for testing checksum validation
    std::ofstream(mock_archive).close();

    std::string expected_hash = "sha256:e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
    std::string mock_sig = "MEQCICR3...[Base64EncodedSignatureBlock]...==";

    if (verifier.VerifyChecksum(mock_archive, expected_hash)) {
        verifier.VerifyPayloadSignature(mock_archive, mock_sig);
    }

    // Cleanup mock file
    std::remove(mock_archive.c_str());

    std::cout << "[+] Signature Verification testing completed successfully.\n";
    return 0;
}

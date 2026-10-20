#pragma once
#include <string>

class SignatureVerifier {
private:
    std::string public_key_path;

public:
    explicit SignatureVerifier(const std::string& pub_key_path = "/etc/nexus/keys/update_pub.pem");

    // Verifies the cryptographic SHA-256 / RSA signature of a downloaded update archive
    bool VerifyPayloadSignature(const std::string& archive_path, const std::string& signature_b64) const;

    // Verifies the cryptographic hash checksum matches the expected manifest hash
    bool VerifyChecksum(const std::string& archive_path, const std::string& expected_sha256) const;
};

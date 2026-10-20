#pragma once
#include <iostream>
#include <string>
#include <vector>

struct CryptoHeader {
    std::string cipher_alg;
    std::string hash_alg;
    uint32_t payload_offset;
    bool is_formatted;
};

class CryptoBlockManager {
private:
    std::string GenerateVolumeKey() const;
    bool VerifyPassphrase(const std::string& passphrase, const std::string& mock_hash) const;

public:
    CryptoBlockManager();

    // Formats a raw block device with a LUKS-compatible encryption header
    bool FormatDevice(const std::string& device_path, const std::string& passphrase);

    // Unlocks the device and creates a mapped decrypted block device
    bool OpenDevice(const std::string& device_path, const std::string& mapped_name, const std::string& passphrase);

    // Locks the device and removes the mapped block device
    bool CloseDevice(const std::string& mapped_name);
};

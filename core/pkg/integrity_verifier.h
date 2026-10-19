#pragma once
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class IntegrityVerifier {
public:
    IntegrityVerifier();

    // Calculates the SHA-256 hash of a given file path
    std::string CalculateSHA256(const std::string& file_path);

    // Compares the calculated hash against the expected hash
    bool VerifyChecksum(const std::string& file_path, const std::string& expected_checksum);
};

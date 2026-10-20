#pragma once
#include <string>

class DeltaPatcher {
public:
    DeltaPatcher();

    // Applies a binary delta patch to a source file/partition to generate the target
    bool ApplyPatch(const std::string& source_path, const std::string& patch_path, const std::string& target_path) const;

    // Verifies the integrity of the newly patched file against an expected checksum
    bool VerifyPatchedIntegrity(const std::string& target_path, const std::string& expected_sha256) const;
};

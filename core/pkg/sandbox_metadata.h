#pragma once
#include <iostream>
#include <string>
#include <vector>

struct SandboxProfile {
    bool requires_network = false;
    bool requires_ipc = false;
    std::vector<std::string> allowed_paths;
};

class PackageSandboxManager {
public:
    PackageSandboxManager();

    // Parses isolation requirements from the package manifest content
    SandboxProfile ParseSandboxMetadata(const std::string& package_name, const std::string& manifest_content);
    
    // Registers the parsed profile with the ApplicationManager / Security Profile Generator
    bool RegisterSandboxProfile(const std::string& package_name, const SandboxProfile& profile);
};

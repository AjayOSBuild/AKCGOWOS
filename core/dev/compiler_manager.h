#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct ToolchainInfo {
    std::string name;
    std::string executable_path;
    std::string version;
    bool is_available = false;
};

class CompilerManager {
private:
    std::unordered_map<std::string, ToolchainInfo> toolchains;

public:
    CompilerManager();
    void DetectToolchains();
    ToolchainInfo GetToolchain(const std::string& name) const;
    std::string CompileCode(const std::string& language, const std::string& source_file, const std::string& output_file);
};

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

struct BuildOptions {
    std::string optimization_level = "-O2"; // -O0, -O1, -O2, -O3, -Os
    bool enable_warnings = true;
    bool treat_warnings_as_errors = false;
    bool enable_address_sanitizer = false;      // Clang/GCC ASan
    bool enable_undefined_sanitizer = false;    // Clang/GCC UBSan
    bool run_static_analysis = false;           // Clang-tidy integration
    std::vector<std::string> include_dirs;
    std::vector<std::string> library_dirs;
    std::vector<std::string> libraries;
};

class CompilerManager {
private:
    std::unordered_map<std::string, ToolchainInfo> toolchains;

public:
    CompilerManager();
    void DetectToolchains();
    ToolchainInfo GetToolchain(const std::string& name) const;
    std::string CompileProject(const std::string& toolchain_key, 
                               const std::vector<std::string>& source_files, 
                               const std::string& output_file, 
                               const BuildOptions& options);
    std::string RunStaticAnalysis(const std::string& source_file, const BuildOptions& options);
};

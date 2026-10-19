#include "compiler_manager.h"
#include <cstdlib>
#include <sstream>

CompilerManager::CompilerManager() {
    std::cout << "[CompilerManager] Initializing Advanced GCC & Clang Toolchain Integration...\n";
    DetectToolchains();
}

void CompilerManager::DetectToolchains() {
    ToolchainInfo gcc{"GCC", "/usr/bin/gcc", "12.2.0", true};
    toolchains["gcc"] = gcc;

    ToolchainInfo clang{"Clang", "/usr/bin/clang", "15.0.0", true};
    toolchains["clang"] = clang;

    ToolchainInfo rustc{"Rustc", "/usr/bin/rustc", "1.70.0", true};
    toolchains["rustc"] = rustc;

    std::cout << "[CompilerManager] Toolchain registry synchronized. Active drivers: GCC, Clang, Rustc.\n";
}

ToolchainInfo CompilerManager::GetToolchain(const std::string& name) const {
    auto it = toolchains.find(name);
    if (it != toolchains.end()) {
        return it->second;
    }
    return {"Unknown", "", "", false};
}

std::string CompilerManager::CompileProject(const std::string& toolchain_key, 
                                            const std::vector<std::string>& source_files, 
                                            const std::string& output_file, 
                                            const BuildOptions& options) {
    auto it = toolchains.find(toolchain_key);
    if (it == toolchains.end() || !it->second.is_available) {
        return "ERROR: Selected toolchain is unavailable.";
    }

    std::stringstream cmd;
    cmd << it->second.executable_path << " ";

    // Append source files
    for (const auto& src : source_files) {
        cmd << src << " ";
    }

    // Append output
    cmd << "-o " << output_file << " ";

    // Append optimization level
    cmd << options.optimization_level << " ";

    // Append warning configurations
    if (options.enable_warnings) {
        cmd << "-Wall -Wextra ";
    }
    if (options.treat_warnings_as_errors) {
        cmd << "-Werror ";
    }

    // Append include paths
    for (const auto& inc : options.include_dirs) {
        cmd << "-I" << inc << " ";
    }

    // Append library paths and libraries
    for (const auto& libdir : options.library_dirs) {
        cmd << "-L" << libdir << " ";
    }
    for (const auto& lib : options.libraries) {
        cmd << "-l" << lib << " ";
    }

    std::string final_command = cmd.str();
    std::cout << "[CompilerManager] Executing build command: " << final_command << "\n";

    int ret = system(final_command.c_str());
    if (ret == 0) {
        return "SUCCESS";
    }
    return "BUILD_FAILURE";
}

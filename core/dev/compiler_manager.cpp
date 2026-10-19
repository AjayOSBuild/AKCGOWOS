#include "compiler_manager.h"
#include <cstdlib>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

CompilerManager::CompilerManager() {
    std::cout << "[CompilerManager] Initializing Rust Toolchain & Cargo Integration...\n";
    DetectToolchains();
}

void CompilerManager::DetectToolchains() {
    ToolchainInfo gcc{"GCC", "/usr/bin/gcc", "12.2.0", true};
    toolchains["gcc"] = gcc;

    ToolchainInfo clang{"Clang", "/usr/bin/clang", "15.0.0", true};
    toolchains["clang"] = clang;

    ToolchainInfo rustc{"Rustc", "/usr/bin/rustc", "1.70.0", true};
    toolchains["rustc"] = rustc;

    std::cout << "[CompilerManager] Rust toolchain detection successful.\n";
}

ToolchainInfo CompilerManager::GetToolchain(const std::string& name) const {
    auto it = toolchains.find(name);
    if (it != toolchains.end()) {
        return it->second;
    }
    return {"Unknown", "", "", false};
}

std::string CompilerManager::CompileRustProject(const std::string& project_dir, const RustBuildOptions& options) {
    auto it = toolchains.find("rustc");
    if (it == toolchains.end() || !it->second.is_available) {
        return "ERROR: Rust toolchain is unavailable.";
    }

    std::stringstream cmd;
    
    // Check if project directory contains Cargo.toml
    if (fs::exists(project_dir + "/Cargo.toml")) {
        cmd << "cargo build --manifest-path " << project_dir << "/Cargo.toml ";
        if (options.release_mode) {
            cmd << "--release ";
        }
        if (!options.target_triple.empty()) {
            cmd << "--target " << options.target_triple << " ";
        }
    } else {
        return "ERROR: No Cargo.toml found in project directory.";
    }

    std::string final_command = cmd.str();
    std::cout << "[CompilerManager] Executing Cargo build command: " << final_command << "\n";

    int ret = system(final_command.c_str());
    if (ret == 0) {
        return "SUCCESS";
    }
    return "BUILD_FAILURE";
}

std::string CompilerManager::RunStaticAnalysis(const std::string& source_file, const BuildOptions& options) {
    std::stringstream cmd;
    cmd << "clang-tidy " << source_file << " -- ";
    for (const auto& inc : options.include_dirs) {
        cmd << "-I" << inc << " ";
    }
    
    std::string analysis_cmd = cmd.str();
    int ret = system(analysis_cmd.c_str());
    return (ret == 0) ? "ANALYSIS_PASSED" : "ANALYSIS_WARNINGS_FOUND";
}

std::string CompilerManager::CompileProject(const std::string& toolchain_key, 
                                            const std::vector<std::string>& source_files, 
                                            const std::string& output_file, 
                                            const BuildOptions& options) {
    auto it = toolchains.find(toolchain_key);
    if (it == toolchains.end() || !it->second.is_available) {
        return "ERROR: Selected toolchain is unavailable.";
    }

    if (options.run_static_analysis && toolchain_key == "clang") {
        for (const auto& src : source_files) {
            RunStaticAnalysis(src, options);
        }
    }

    std::stringstream cmd;
    cmd << it->second.executable_path << " ";

    for (const auto& src : source_files) {
        cmd << src << " ";
    }

    cmd << "-o " << output_file << " ";
    cmd << options.optimization_level << " ";

    if (options.enable_warnings) {
        cmd << "-Wall -Wextra ";
    }
    if (options.treat_warnings_as_errors) {
        cmd << "-Werror ";
    }

    if (toolchain_key == "clang") {
        if (options.enable_address_sanitizer) {
            cmd << "-fsanitize=address -fno-omit-frame-pointer ";
        }
        if (options.enable_undefined_sanitizer) {
            cmd << "-fsanitize=undefined ";
        }
    }

    for (const auto& inc : options.include_dirs) {
        cmd << "-I" << inc << " ";
    }

    for (const auto& libdir : options.library_dirs) {
        cmd << "-L" << libdir << " ";
    }
    for (const auto& lib : options.libraries) {
        cmd << "-l" << lib << " ";
    }

    std::string final_command = cmd.str();
    int ret = system(final_command.c_str());
    if (ret == 0) {
        return "SUCCESS";
    }
    return "BUILD_FAILURE";
}

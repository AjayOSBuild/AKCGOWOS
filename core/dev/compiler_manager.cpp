#include "compiler_manager.h"
#include <cstdlib>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

CompilerManager::CompilerManager() {
    std::cout << "[CompilerManager] Initializing Python & Scripting Toolchain Support...\n";
    DetectToolchains();
}

void CompilerManager::DetectToolchains() {
    ToolchainInfo gcc{"GCC", "/usr/bin/gcc", "12.2.0", true};
    toolchains["gcc"] = gcc;

    ToolchainInfo clang{"Clang", "/usr/bin/clang", "15.0.0", true};
    toolchains["clang"] = clang;

    ToolchainInfo rustc{"Rustc", "/usr/bin/rustc", "1.70.0", true};
    toolchains["rustc"] = rustc;

    ToolchainInfo python{"Python3", "/usr/bin/python3", "3.10.12", true};
    toolchains["python3"] = python;

    std::cout << "[CompilerManager] Python toolchain detection successful.\n";
}

ToolchainInfo CompilerManager::GetToolchain(const std::string& name) const {
    auto it = toolchains.find(name);
    if (it != toolchains.end()) {
        return it->second;
    }
    return {"Unknown", "", "", false};
}

std::string CompilerManager::RunPythonScript(const std::string& script_path, const std::vector<std::string>& args) {
    auto it = toolchains.find("python3");
    if (it == toolchains.end() || !it->second.is_available) {
        return "ERROR: Python3 toolchain is unavailable.";
    }

    if (!fs::exists(script_path)) {
        return "ERROR: Script file not found: " + script_path;
    }

    std::stringstream cmd;
    cmd << it->second.executable_path << " " << script_path << " ";
    for (const auto& arg : args) {
        cmd << arg << " ";
    }

    std::string final_command = cmd.str();
    std::cout << "[CompilerManager] Executing Python script: " << final_command << "\n";

    int ret = system(final_command.c_str());
    return (ret == 0) ? "SUCCESS" : "SCRIPT_EXECUTION_FAILURE";
}

std::string CompilerManager::CreatePythonVirtualEnv(const std::string& venv_path) {
    auto it = toolchains.find("python3");
    if (it == toolchains.end() || !it->second.is_available) {
        return "ERROR: Python3 toolchain is unavailable.";
    }

    std::stringstream cmd;
    cmd << it->second.executable_path << " -m venv " << venv_path;

    std::string final_command = cmd.str();
    std::cout << "[CompilerManager] Creating Python virtual environment at " << venv_path << ": " << final_command << "\n";

    int ret = system(final_command.c_str());
    return (ret == 0) ? "SUCCESS" : "VENV_CREATION_FAILURE";
}

std::string CompilerManager::CompileRustProject(const std::string& project_dir, const RustBuildOptions& options) {
    auto it = toolchains.find("rustc");
    if (it == toolchains.end() || !it->second.is_available) {
        return "ERROR: Rust toolchain is unavailable.";
    }

    std::stringstream cmd;
    if (fs::exists(project_dir + "/Cargo.toml")) {
        cmd << "cargo build --manifest-path " << project_dir << "/Cargo.toml ";
        if (options.release_mode) cmd << "--release ";
        if (!options.target_triple.empty()) cmd << "--target " << options.target_triple << " ";
    } else {
        return "ERROR: No Cargo.toml found in project directory.";
    }

    int ret = system(cmd.str().c_str());
    return (ret == 0) ? "SUCCESS" : "BUILD_FAILURE";
}

std::string CompilerManager::RunStaticAnalysis(const std::string& source_file, const BuildOptions& options) {
    std::stringstream cmd;
    cmd << "clang-tidy " << source_file << " -- ";
    for (const auto& inc : options.include_dirs) {
        cmd << "-I" << inc << " ";
    }
    int ret = system(cmd.str().c_str());
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
        for (const auto& src : source_files) RunStaticAnalysis(src, options);
    }

    std::stringstream cmd;
    cmd << it->second.executable_path << " ";
    for (const auto& src : source_files) cmd << src << " ";
    cmd << "-o " << output_file << " " << options.optimization_level << " ";
    if (options.enable_warnings) cmd << "-Wall -Wextra ";
    if (options.treat_warnings_as_errors) cmd << "-Werror ";

    if (toolchain_key == "clang") {
        if (options.enable_address_sanitizer) cmd << "-fsanitize=address -fno-omit-frame-pointer ";
        if (options.enable_undefined_sanitizer) cmd << "-fsanitize=undefined ";
    }

    for (const auto& inc : options.include_dirs) cmd << "-I" << inc << " ";
    for (const auto& libdir : options.library_dirs) cmd << "-L" << libdir << " ";
    for (const auto& lib : options.libraries) cmd << "-l" << lib << " ";

    int ret = system(cmd.str().c_str());
    return (ret == 0) ? "SUCCESS" : "BUILD_FAILURE";
}

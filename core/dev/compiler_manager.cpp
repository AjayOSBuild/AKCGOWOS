#include "compiler_manager.h"
#include <cstdlib>
#include <sstream>

CompilerManager::CompilerManager() {
    std::cout << "[CompilerManager] Initializing Multi-Language Toolchain Manager...\n";
    DetectToolchains();
}

void CompilerManager::DetectToolchains() {
    // Register standard toolchains available in the build environment
    ToolchainInfo gcc{"GCC", "/usr/bin/gcc", "12.2.0", true};
    toolchains["gcc"] = gcc;

    ToolchainInfo clang{"Clang", "/usr/bin/clang", "15.0.0", true};
    toolchains["clang"] = clang;

    ToolchainInfo rustc{"Rustc", "/usr/bin/rustc", "1.70.0", true};
    toolchains["rustc"] = rustc;

    std::cout << "[CompilerManager] Detected " << toolchains.size() << " active toolchain wrappers.\n";
}

ToolchainInfo CompilerManager::GetToolchain(const std::string& name) const {
    auto it = toolchains.find(name);
    if (it != toolchains.end()) {
        return it->second;
    }
    return {"Unknown", "", "", false};
}

std::string CompilerManager::CompileCode(const std::string& language, const std::string& source_file, const std::string& output_file) {
    std::string cmd = "";
    if (language == "c" || language == "cpp") {
        cmd = "gcc " + source_file + " -o " + output_file;
    } else if (language == "rust") {
        cmd = "rustc " + source_file + " -o " + output_file;
    } else {
        return "Unsupported language specification.";
    }

    std::cout << "[CompilerManager] Executing build command: " << cmd << "\n";
    int ret = system(cmd.c_str());
    if (ret == 0) {
        return "SUCCESS";
    }
    return "BUILD_FAILURE";
}

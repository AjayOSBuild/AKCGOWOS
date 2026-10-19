#include <iostream>
#include <string>
#include "pe_loader.h"
#include "path_redirector.h"
#include "registry_virtualizer.h"
#include "dx_translator.h"
#include "win32_translator.h"

int main(int argc, char* argv[]) {
    std::cout << "=== Nexus OS Windows Compatibility Subsystem (Ring 3) ===\n";

    if (argc < 2) {
        std::cout << "Usage: win_runner <path_to_windows_executable.exe>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::cout << "[WinRunner] Initiating launch sequence for: " << input_path << "\n";

    // 1. Path Redirection & Sandbox Verification
    Kernel32API::PathRedirector redirector;
    std::string native_path = redirector.TranslatePath(input_path);
    std::cout << "[WinRunner] Mapped to sandbox path: " << native_path << "\n";

    // 2. Pre-seed Virtualized Registry State
    Advapi32API::RegistryVirtualizer registry;
    HKEY hKey = nullptr;
    registry.OpenKey(HKEY_CURRENT_USER, "Software\\NexusOS\\Compat", &hKey);

    // 3. Initialize Direct3D / Vulkan Translation Pipeline
    DXVKTranslator dx_pipeline;

    // 4. PE Header Parsing & Virtual Memory Mapping
    PELoader loader(native_path);
    if (!loader.LoadAndMap()) {
        std::cerr << "[!] Failed to map PE binary into user-space.\n";
        return 1;
    }

    // 5. Transfer Execution to PE Entrypoint
    loader.ExecuteEntrypoint();

    std::cout << "[+] Windows executable execution lifecycle completed cleanly.\n";
    return 0;
}

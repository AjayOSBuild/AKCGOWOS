#pragma once
#include "vm_manager.h"
#include <string>

class AppSandbox {
private:
    VmManager vm_mgr;
    std::string app_name;
    std::string executable_path;
    std::string sandbox_vm_name;
    bool is_running;

public:
    AppSandbox(const std::string& name, const std::string& exec_path);

    // Provisions a lightweight microVM environment for the application
    bool Initialize();

    // Starts the sandboxed application
    bool Run();

    // Terminates the sandbox and cleans up ephemeral resources
    bool Stop();
};

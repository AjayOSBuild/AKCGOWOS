#pragma once
#include "vm_manager.h"
#include <string>

class VmManagementUI {
private:
    VmManager& vm_mgr;
    bool is_running;

    void DisplayHeader() const;
    void DisplayMenu() const;
    void ProcessCommand(const std::string& cmd);

public:
    explicit VmManagementUI(VmManager& manager);

    // Starts the interactive UI event loop
    void Run();
};

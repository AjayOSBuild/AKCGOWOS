#include <iostream>
#include "task_manager.h"

int main() {
    std::cout << "=== OS Project Advanced System Task Manager Utility ===\n";
    SystemTaskManager task_mgr;
    
    // Refresh to verify procfs parsing
    task_mgr.RefreshSystemState();
    
    std::cout << "[+] Advanced Task Manager utility initialized successfully.\n";
    return 0;
}

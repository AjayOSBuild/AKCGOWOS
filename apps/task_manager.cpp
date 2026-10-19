#include <iostream>
#include "task_manager.h"

int main() {
    std::cout << "=== OS Project Task Manager App ===\n";
    TaskManagerApp task_manager;
    
    // Refresh stats to verify procfs parsing
    task_manager.Refresh();
    
    std::cout << "[+] Task Manager application initialized successfully.\n";
    return 0;
}

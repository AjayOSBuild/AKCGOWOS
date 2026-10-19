#include <iostream>
#include "system_terminal.h"

int main() {
    std::cout << "=== OS Project System Administration Terminal Utility ===\n";
    SystemTerminalUtility term;
    
    // Test issuing maintenance commands
    term.ExecuteCommand("help");
    term.ExecuteCommand("sysinfo");
    term.ExecuteCommand("services");
    
    std::cout << "[+] System Terminal utility test completed successfully.\n";
    return 0;
}

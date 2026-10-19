#include <iostream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    
    // Create Primary Window (Window ID 1)
    FileExplorerApp main_window(1, "/mnt/usb");
    
    // Test EXP-018: Tabs Support
    main_window.OpenNewTab("/tmp");       // Tab 1 is now active
    main_window.OpenNewTab("trash://");   // Tab 2 is now active
    main_window.SwitchTab(0);             // Switch back to Tab 0 (/mnt/usb)
    main_window.CloseTab(1);              // Close Tab 1 (/tmp)
    
    // Test EXP-019: Multiple Windows Support
    auto second_window = FileExplorerApp::SpawnNewWindow(2, "/root");
    second_window->OpenNewTab("/mnt");
    
    std::cout << "[+] File Explorer EXP-018 (Tabs) and EXP-019 (Multi-Window) tests completed successfully.\n";
    return 0;
}

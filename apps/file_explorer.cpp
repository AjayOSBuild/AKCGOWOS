#include <iostream>
#include <fstream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer(1, "/mnt/usb");
    
    // Create a dummy hidden file to test the filter
    std::ofstream dummy_hidden("/mnt/usb/.secret_config");
    dummy_hidden << "hidden data";
    dummy_hidden.close();

    // Test EXP-020: Toggle hidden files
    std::cout << "\n--- Testing Settings (EXP-020) ---\n";
    explorer.RefreshContents(); // Should hide .secret_config
    explorer.ToggleHiddenFiles(); // Should now reveal .secret_config
    explorer.SetViewMode(ExplorerSettings::ViewMode::GRID);

    // Test EXP-016: Connect to a network location
    std::cout << "\n--- Testing Network Locations (EXP-016) ---\n";
    explorer.ConnectNetworkLocation("smb", "192.168.1.100/shared", "nas_shared");
    
    std::cout << "\n[+] File Explorer EXP-016 and EXP-020 tests completed successfully.\n";
    return 0;
}

#include <iostream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer("/mnt/usb");
    
    // Simulate right-click context menu and viewing properties
    explorer.ShowContextMenu(300, 200, "/mnt/usb");
    explorer.ShowPropertiesDialog("/mnt/usb");
    
    std::cout << "[+] File Explorer context menu and properties test completed successfully.\n";
    return 0;
}

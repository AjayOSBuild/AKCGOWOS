#include <iostream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer("/mnt/usb");
    
    // Simulate user search query against cached index
    explorer.HandleSearch("doc");
    
    std::cout << "[+] File Explorer application loop exited cleanly.\n";
    return 0;
}

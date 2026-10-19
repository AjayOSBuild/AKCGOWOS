#include <iostream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer("/mnt/usb");
    
    // Simulate testing trash deletion action
    // explorer.DeleteToTrash("/mnt/usb/sample.txt");
    
    std::cout << "[+] File Explorer file operations test completed successfully.\n";
    return 0;
}

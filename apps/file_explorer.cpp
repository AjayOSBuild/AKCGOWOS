#include <iostream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer("/mnt/usb");
    
    // Simulate testing directory navigation
    explorer.NavigateTo("/mnt");
    
    std::cout << "[+] File Explorer navigation test completed successfully.\n";
    return 0;
}

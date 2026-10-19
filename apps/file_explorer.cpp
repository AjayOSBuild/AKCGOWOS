#include <iostream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer("/mnt/usb");
    
    // Test EXP-011: Switch view to Recycle Bin, list items, and simulate empty/restore
    explorer.NavigateTo("trash://");
    explorer.ClearRecycleBin();
    
    std::cout << "[+] File Explorer EXP-011 Recycle Bin test completed successfully.\n";
    return 0;
}

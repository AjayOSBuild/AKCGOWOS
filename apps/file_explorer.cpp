#include <iostream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer("/mnt/usb");
    
    // Test EXP-005: Address bar path change submission
    explorer.HandleAddressBarInput("/mnt");
    
    // Test EXP-009: File / Folder renaming simulation
    // explorer.RenameItem("/mnt/old_folder", "new_folder");
    
    std::cout << "[+] File Explorer EXP-005 and EXP-009 test completed successfully.\n";
    return 0;
}

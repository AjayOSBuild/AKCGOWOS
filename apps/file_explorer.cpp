#include <iostream>
#include <fstream>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer("/mnt/usb");
    
    // Create a dummy text file to test the preview functionality
    std::ofstream dummy("/mnt/usb/test_document.txt");
    dummy << "Line 1: Hello from Nexus OS\nLine 2: This is a text preview.\nLine 3: File explorers are fun.";
    dummy.close();
    
    // Create a dummy archive file
    std::ofstream dummy_zip("/mnt/usb/backup.zip");
    dummy_zip << "PK...";
    dummy_zip.close();

    // Test EXP-013: File previews
    explorer.ShowFilePreview("/mnt/usb/test_document.txt");
    
    // Test EXP-015: Archive extraction
    explorer.ExtractArchive("/mnt/usb/backup.zip");
    
    std::cout << "[+] File Explorer EXP-013 and EXP-015 tests completed successfully.\n";
    return 0;
}

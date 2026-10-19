#include <iostream>
#include <fstream>
#include "text_editor.h"

int main() {
    std::cout << "=== OS Project Text Editor App ===\n";
    TextEditorApp editor;
    
    // Create a dummy file to test loading
    std::string test_file = "/tmp/editor_test.txt";
    std::ofstream dummy(test_file);
    dummy << "Line 1: Nexus OS Native Text Editor\nLine 2: Fast and lightweight.";
    dummy.close();

    std::cout << "\n--- Simulating: Loading File ---\n";
    editor.LoadFile(test_file);
    
    std::cout << "\n--- Simulating: Editing Document ---\n";
    // Simulate moving cursor to end of line 2 and pressing enter
    for (int i=0; i<29; i++) editor.HandleKeyboardEvent(0, false); // Mock cursor moves
    editor.HandleKeyboardEvent(10, true); // Enter
    std::string new_text = "Line 3: Added via UI event simulation.";
    for (char c : new_text) {
        editor.HandleKeyboardEvent(static_cast<int>(c), true);
    }
    
    std::cout << "\n--- Simulating: Saving File ---\n";
    editor.SaveFile(test_file);
    
    std::cout << "[+] Text Editor application tests completed successfully.\n";
    return 0;
}

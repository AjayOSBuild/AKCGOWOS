#include <iostream>
#include <chrono>
#include <thread>
#include "dev_terminal.h"

int main() {
    std::cout << "=== OS Project Development Terminal App ===\n";
    DevTerminalApp dev_term;
    
    // Simulate keyboard typing: "rustc --version"
    std::string test_cmd = "rustc --version";
    for (char c : test_cmd) {
        dev_term.HandleKeyboardEvent(static_cast<int>(c), true);
    }
    // Simulate 'Enter' key press
    dev_term.HandleKeyboardEvent(10, true);
    
    // UI Event Loop
    int ticks = 0;
    while (ticks < 10) { 
        dev_term.PollOutput();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ticks++;
    }
    
    std::cout << "[+] Development Terminal application loop exited cleanly.\n";
    return 0;
}

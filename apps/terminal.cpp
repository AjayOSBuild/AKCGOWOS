#include <iostream>
#include <chrono>
#include <thread>
#include "terminal.h"

int main() {
    std::cout << "=== OS Project Terminal App ===\n";
    TerminalApp terminal;
    
    // Simulate keyboard typing: "echo hello"
    std::string test_cmd = "echo hello";
    for (char c : test_cmd) {
        terminal.HandleKeyboardEvent(static_cast<int>(c), true);
    }
    // Simulate 'Enter' key press
    terminal.HandleKeyboardEvent(10, true);
    
    // UI Event Loop
    int ticks = 0;
    while (ticks < 10) { // Run for a few cycles to capture output
        terminal.PollOutput();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ticks++;
    }
    
    std::cout << "[+] Terminal application loop exited cleanly.\n";
    return 0;
}

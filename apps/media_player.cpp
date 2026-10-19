#include <iostream>
#include <thread>
#include <chrono>
#include "media_player.h"

int main() {
    std::cout << "=== OS Project Media Player App ===\n";
    MediaPlayerApp player;
    
    // Simulate User Workflow
    std::cout << "\n--- Simulating: Loading and Playing Video ---\n";
    player.LoadMedia("/mnt/usb/sample_video.mp4");
    player.Play();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    player.Seek(45.0);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    player.Pause();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    player.Stop();
    
    std::cout << "[+] Media Player application tests completed successfully.\n";
    return 0;
}

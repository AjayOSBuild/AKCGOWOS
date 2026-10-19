#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include "file_explorer.h"

int main() {
    std::cout << "=== OS Project File Explorer App ===\n";
    FileExplorerApp explorer(1, "/mnt/usb");
    
    // Create a 2MB dummy file to simulate a large operation
    std::string big_file_path = "/mnt/usb/large_video.mp4";
    std::ofstream big_file(big_file_path, std::ios::binary);
    std::vector<char> dummy_data(1024 * 1024 * 2, 'A'); // 2MB
    big_file.write(dummy_data.data(), dummy_data.size());
    big_file.close();

    // Test EXP: Large Operations (Progress Tracking & Cancellation)
    std::cout << "\n--- Testing Large Operations (Progress & Cancellation) ---\n";
    
    std::string dest_path = "/mnt/usb/large_video_copy.mp4";
    explorer.AsyncCopyItem(big_file_path, dest_path);
    
    // UI Progress Polling Loop
    while (explorer.current_worker && explorer.current_worker->is_running) {
        uint64_t copied = explorer.current_worker->copied_bytes;
        uint64_t total = explorer.current_worker->total_bytes;
        
        if (total > 0) {
            double percentage = (static_cast<double>(copied) / total) * 100.0;
            std::cout << "\r[UI] Copying: " << std::fixed << std::setprecision(1) << percentage 
                      << "% (" << copied / 1024 << " KB / " << total / 1024 << " KB)  " << std::flush;
        }

        // Simulate user clicking "Cancel" at 50%
        if (copied > (total / 2) && !explorer.current_worker->cancel_requested) {
            std::cout << "\n";
            explorer.current_worker->Cancel();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    std::cout << "\n[+] File Explorer Large Operations test completed successfully.\n";
    return 0;
}

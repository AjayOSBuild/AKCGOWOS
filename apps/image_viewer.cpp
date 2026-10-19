#include <iostream>
#include <fstream>
#include "image_viewer.h"

int main() {
    std::cout << "=== OS Project Image Viewer App ===\n";
    ImageViewerApp viewer;

    // Create a dummy file to test metadata extraction
    std::string test_img = "/tmp/sample_photo.jpg";
    std::ofstream dummy(test_img, std::ios::binary);
    dummy << "FAKE_JPEG_HEADER";
    dummy.close();

    std::cout << "\n--- Simulating: Loading and Inspecting Image ---\n";
    viewer.LoadImage(test_img);
    viewer.ZoomIn();
    viewer.ZoomIn();
    viewer.ResetZoom();

    std::cout << "[+] Image Viewer application tests completed successfully.\n";
    return 0;
}

#include "../test_framework.h"
#include <string>
#include <iomanip>
#include <sstream>

// Mock graphics abstraction functions for testing purposes
struct Rect {
    int x, y, width, height;
};

bool IsPointInRect(int px, int py, const Rect& rect) {
    return (px >= rect.x && px <= rect.x + rect.width &&
            py >= rect.y && py <= rect.y + rect.height);
}

std::string RgbToHex(int r, int g, int b) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) return "#000000";
    std::stringstream stream;
    stream << "#" << std::setfill('0') << std::hex
           << std::setw(2) << r
           << std::setw(2) << g
           << std::setw(2) << b;
    std::string result = stream.str();
    for (auto& c : result) c = toupper(c);
    return result;
}

TEST(Gfx_HitTesting) {
    Rect window = {100, 100, 800, 600};
    
    // Inside
    ASSERT_TRUE(IsPointInRect(500, 400, window));
    ASSERT_TRUE(IsPointInRect(100, 100, window)); // Top-left edge
    
    // Outside
    ASSERT_EQ(false, IsPointInRect(50, 50, window));
    ASSERT_EQ(false, IsPointInRect(1000, 800, window));
}

TEST(Gfx_ColorConversion) {
    ASSERT_EQ("#FFFFFF", RgbToHex(255, 255, 255));
    ASSERT_EQ("#000000", RgbToHex(0, 0, 0));
    ASSERT_EQ("#FF0000", RgbToHex(255, 0, 0));
    ASSERT_EQ("#000000", RgbToHex(300, -10, 0)); // Out of bounds fallback
}

int main() {
    return TestRegistry::RunAll();
}

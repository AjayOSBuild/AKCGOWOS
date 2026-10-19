#pragma once
#include <iostream>
#include <string>

class AndroidInputBridge {
private:
    std::string virtual_touch_device;
    std::string virtual_keyboard_device;
    int touch_fd = -1;
    int keyboard_fd = -1;

public:
    AndroidInputBridge(const std::string& touch_dev = "/dev/input/event10", 
                       const std::string& kb_dev = "/dev/input/event11");
    ~AndroidInputBridge();

    // Translates Nexus OS pointer events into Android multi-touch protocol (Type B) events
    bool InjectTouchEvent(int x, int y, bool pressed);

    // Translates Nexus OS key events into Android keycodes
    bool InjectKeyEvent(int key_code, bool pressed);
};

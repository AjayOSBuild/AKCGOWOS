#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

struct TrayStatus {
    bool network_connected = false;
    int battery_level = 100;
    int volume_level = 50;
};

class SystemTray {
private:
    TrayStatus current_status;

    std::string GetCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        char buf[20];
        // Format: HH:MM
        std::strftime(buf, sizeof(buf), "%H:%M", std::localtime(&now_c));
        return std::string(buf);
    }

public:
    SystemTray() {
        std::cout << "[SystemTray] Initializing status icons and system clock...\n";
    }

    void UpdateStatus(bool net, int batt, int vol) {
        current_status.network_connected = net;
        current_status.battery_level = batt;
        current_status.volume_level = vol;
    }

    void Render() {
        // In full execution, this submits drawing commands to the Compositor via IPC
        // to render the Wi-Fi, Battery, and Volume icons on the far right of the taskbar,
        // followed by the current time.
    }
};

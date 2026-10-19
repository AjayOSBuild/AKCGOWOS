#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

struct Notification {
    std::string title;
    std::string message;
    std::string app_source;
    long long timestamp;
};

class NotificationCenter {
private:
    bool is_open = false;
    std::vector<Notification> history;

public:
    NotificationCenter() {
        std::cout << "[NotificationCenter] Initializing notification UI and event history panel...\n";
    }

    void Toggle() {
        is_open = !is_open;
        std::cout << "[NotificationCenter] Panel state changed to: " << (is_open ? "OPEN" : "CLOSED") << "\n";
    }

    bool IsOpen() const {
        return is_open;
    }

    void PushNotification(const std::string& app_source, const std::string& title, const std::string& message) {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        long long ts = std::chrono::duration_cast<std::chrono::seconds>(now).count();
        history.push_back({title, message, app_source, ts});
        
        std::cout << "[NotificationCenter] New Alert from [" << app_source << "]: " << title << " - " << message << "\n";
        // In full execution, this triggers a temporary "toast" popup overlay for a few seconds.
    }

    void Render() {
        if (!is_open) return;
        // Render pass for the side panel containing notification history
    }
};

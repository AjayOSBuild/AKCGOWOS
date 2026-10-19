#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <stdlib.h>

class TerminalApp {
private:
    int master_fd = -1;
    pid_t shell_pid = -1;
    std::deque<std::string> scrollback_buffer;
    std::string current_input;
    const int MAX_SCROLLBACK = 1000;

    bool InitializePTY() {
        master_fd = posix_openpt(O_RDWR | O_NOCTTY);
        if (master_fd < 0) {
            std::cerr << "[!] Failed to open pseudo-terminal master.\n";
            return false;
        }

        if (grantpt(master_fd) != 0 || unlockpt(master_fd) != 0) {
            std::cerr << "[!] Failed to grant/unlock pseudo-terminal.\n";
            close(master_fd);
            return false;
        }

        char* slave_name = ptsname(master_fd);
        if (!slave_name) {
            std::cerr << "[!] Failed to get pseudo-terminal slave name.\n";
            close(master_fd);
            return false;
        }

        shell_pid = fork();
        if (shell_pid == 0) {
            // Child process: The Shell
            close(master_fd);
            
            // Open slave PTY and duplicate standard file descriptors
            int slave_fd = open(slave_name, O_RDWR);
            dup2(slave_fd, STDIN_FILENO);
            dup2(slave_fd, STDOUT_FILENO);
            dup2(slave_fd, STDERR_FILENO);
            close(slave_fd);

            // Set new session ID and execute shell
            setsid();
            ioctl(0, TIOCSCTTY, 1);
            execl("/bin/sh", "sh", nullptr);
            
            // If execl fails
            exit(1);
        } else if (shell_pid > 0) {
            // Parent process: The Terminal UI
            // Set master PTY to non-blocking mode for smooth UI rendering
            int flags = fcntl(master_fd, F_GETFL, 0);
            fcntl(master_fd, F_SETFL, flags | O_NONBLOCK);
            
            std::cout << "[Terminal] Spawned shell process (PID: " << shell_pid << ") on " << slave_name << "\n";
            return true;
        }

        std::cerr << "[!] Fork failed.\n";
        return false;
    }

public:
    TerminalApp() {
        std::cout << "[Terminal] Initializing native terminal emulator...\n";
        if (InitializePTY()) {
            scrollback_buffer.push_back("Welcome to Nexus OS Terminal.");
        } else {
            scrollback_buffer.push_back("[Error] Failed to initialize PTY backend.");
        }
    }

    ~TerminalApp() {
        if (master_fd >= 0) close(master_fd);
        if (shell_pid > 0) kill(shell_pid, SIGTERM);
    }

    void PollOutput() {
        if (master_fd < 0) return;

        char buffer[256];
        ssize_t bytes_read = read(master_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            // Simplified rendering: split by newlines and push to buffer
            std::string output(buffer);
            size_t pos = 0;
            while ((pos = output.find('\n')) != std::string::npos) {
                scrollback_buffer.push_back(output.substr(0, pos));
                output.erase(0, pos + 1);
            }
            if (!output.empty()) {
                scrollback_buffer.push_back(output);
            }

            // Trim buffer to prevent unbounded memory growth
            while (scrollback_buffer.size() > MAX_SCROLLBACK) {
                scrollback_buffer.pop_front();
            }
        }
    }

    void SendInput(const std::string& input) {
        if (master_fd < 0) return;
        std::string command = input + "\n";
        write(master_fd, command.c_str(), command.length());
    }

    void HandleKeyboardEvent(int key_code, bool pressed) {
        if (!pressed) return;
        
        // Simplified ASCII input mapping
        if (key_code == 8) { // Backspace
            if (!current_input.empty()) current_input.pop_back();
        } else if (key_code == 10 || key_code == 13) { // Enter
            scrollback_buffer.push_back("$ " + current_input);
            SendInput(current_input);
            current_input.clear();
        } else if (key_code >= 32 && key_code <= 126) {
            current_input += static_cast<char>(key_code);
        }
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Draw terminal background
        gfx.FillRect(0, 0, 800, 600, 0xFF1E1E1E); // Dark gray/black
        
        // Draw terminal chrome/border
        gfx.DrawRect(0, 0, 800, 600, 0xFF444444, 2);

        // Render scrollback buffer
        int start_y = 20;
        int max_lines = 500 / 15; // Rough estimate of visible lines
        int start_idx = std::max(0, static_cast<int>(scrollback_buffer.size()) - max_lines);
        
        for (size_t i = start_idx; i < scrollback_buffer.size(); ++i) {
            font_engine.DrawText(scrollback_buffer[i], 10, start_y, 12, 0xFFCCCCCC);
            start_y += 15;
        }

        // Render current input line and cursor
        font_engine.DrawText("$ " + current_input, 10, start_y, 12, 0xFFFFFFFF);
        gfx.FillRect(10 + 16 + (current_input.length() * 8), start_y + 2, 8, 12, 0xFFFFFFFF);
    }
};

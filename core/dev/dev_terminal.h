#pragma once
#include "../../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <stdlib.h>

class DevTerminalApp {
private:
    int master_fd = -1;
    pid_t shell_pid = -1;
    std::deque<std::string> scrollback_buffer;
    std::string current_input;
    const int MAX_SCROLLBACK = 2000; // Larger scrollback for compilation logs

    void SetupDevEnvironment() {
        std::cout << "[DevTerminal] Injecting developer environment variables...\n";
        // Prepend standard SDK and toolchain paths to the environment
        setenv("PATH", "/opt/nexus/sdks/active/bin:/usr/local/bin:/usr/bin:/bin:/sbin", 1);
        setenv("C_INCLUDE_PATH", "/opt/nexus/sdks/active/include", 1);
        setenv("CPLUS_INCLUDE_PATH", "/opt/nexus/sdks/active/include/c++", 1);
        setenv("LD_LIBRARY_PATH", "/opt/nexus/sdks/active/lib", 1);
        setenv("PKG_CONFIG_PATH", "/opt/nexus/sdks/active/lib/pkgconfig", 1);
        setenv("RUSTUP_HOME", "/opt/nexus/sdks/rustup", 1);
        setenv("CARGO_HOME", "/opt/nexus/sdks/cargo", 1);
        setenv("NEXUS_DEV_ENV", "1", 1);
    }

    bool InitializePTY() {
        master_fd = posix_openpt(O_RDWR | O_NOCTTY);
        if (master_fd < 0) return false;
        if (grantpt(master_fd) != 0 || unlockpt(master_fd) != 0) {
            close(master_fd);
            return false;
        }

        char* slave_name = ptsname(master_fd);
        if (!slave_name) {
            close(master_fd);
            return false;
        }

        shell_pid = fork();
        if (shell_pid == 0) {
            // Child process: The Shell
            close(master_fd);
            
            int slave_fd = open(slave_name, O_RDWR);
            dup2(slave_fd, STDIN_FILENO);
            dup2(slave_fd, STDOUT_FILENO);
            dup2(slave_fd, STDERR_FILENO);
            close(slave_fd);

            setsid();
            ioctl(0, TIOCSCTTY, 1);
            
            // Inject environment before executing shell
            SetupDevEnvironment();
            
            execl("/bin/sh", "sh", nullptr);
            exit(1);
        } else if (shell_pid > 0) {
            // Parent process: The Terminal UI
            int flags = fcntl(master_fd, F_GETFL, 0);
            fcntl(master_fd, F_SETFL, flags | O_NONBLOCK);
            std::cout << "[DevTerminal] Spawned developer shell (PID: " << shell_pid << ") on " << slave_name << "\n";
            return true;
        }
        return false;
    }

public:
    DevTerminalApp() {
        std::cout << "[DevTerminal] Initializing native Development Terminal...\n";
        if (InitializePTY()) {
            scrollback_buffer.push_back("Nexus OS Developer Command Prompt");
            scrollback_buffer.push_back("Environment: GCC, Clang, Rust, Python initialized.");
        } else {
            scrollback_buffer.push_back("[Error] Failed to initialize PTY backend.");
        }
    }

    ~DevTerminalApp() {
        if (master_fd >= 0) close(master_fd);
        if (shell_pid > 0) kill(shell_pid, SIGTERM);
    }

    void PollOutput() {
        if (master_fd < 0) return;
        char buffer[512];
        ssize_t bytes_read = read(master_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::string output(buffer);
            size_t pos = 0;
            while ((pos = output.find('\n')) != std::string::npos) {
                scrollback_buffer.push_back(output.substr(0, pos));
                output.erase(0, pos + 1);
            }
            if (!output.empty()) scrollback_buffer.push_back(output);
            while (scrollback_buffer.size() > MAX_SCROLLBACK) scrollback_buffer.pop_front();
        }
    }

    void SendInput(const std::string& input) {
        if (master_fd < 0) return;
        std::string command = input + "\n";
        write(master_fd, command.c_str(), command.length());
    }

    void HandleKeyboardEvent(int key_code, bool pressed) {
        if (!pressed) return;
        if (key_code == 8) {
            if (!current_input.empty()) current_input.pop_back();
        } else if (key_code == 10 || key_code == 13) {
            scrollback_buffer.push_back("dev@nexus:~/workspace$ " + current_input);
            SendInput(current_input);
            current_input.clear();
        } else if (key_code >= 32 && key_code <= 126) {
            current_input += static_cast<char>(key_code);
        }
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        gfx.FillRect(0, 0, 900, 700, 0xFF0A0A2A); // Midnight blue developer theme
        gfx.DrawRect(0, 0, 900, 700, 0xFF444488, 2);

        int start_y = 20;
        int max_lines = 600 / 15;
        int start_idx = std::max(0, static_cast<int>(scrollback_buffer.size()) - max_lines);
        
        for (size_t i = start_idx; i < scrollback_buffer.size(); ++i) {
            font_engine.DrawText(scrollback_buffer[i], 10, start_y, 13, 0xFFE0E0E0);
            start_y += 15;
        }

        font_engine.DrawText("dev@nexus:~/workspace$ " + current_input, 10, start_y, 13, 0xFF00FFCC);
        gfx.FillRect(10 + 175 + (current_input.length() * 8), start_y + 2, 8, 12, 0xFF00FFCC);
    }
};

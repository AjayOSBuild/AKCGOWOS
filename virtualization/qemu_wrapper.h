#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>

class QemuWrapper {
private:
    std::string qemu_binary;
    std::vector<std::string> arguments;
    pid_t qemu_pid;

public:
    QemuWrapper(const std::string& binary_path = "/usr/bin/qemu-system-x86_64");

    // Adds a command line argument to the QEMU invocation
    void AddArgument(const std::string& arg);

    // Clears all configured arguments
    void ClearArguments();

    // Launches the QEMU process
    bool Start();

    // Sends a SIGTERM to gracefully stop the QEMU process
    bool Stop();

    // Checks if the QEMU process is currently running
    bool IsRunning() const;
    
    // Returns the PID of the managed QEMU instance
    pid_t GetPid() const;
};

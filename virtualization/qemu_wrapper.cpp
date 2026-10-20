#include "qemu_wrapper.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <iostream>

QemuWrapper::QemuWrapper(const std::string& binary_path) 
    : qemu_binary(binary_path), qemu_pid(-1) {
    std::cout << "[QEMU-Wrapper] Initialized with binary: " << qemu_binary << "\n";
}

void QemuWrapper::AddArgument(const std::string& arg) {
    arguments.push_back(arg);
}

void QemuWrapper::ClearArguments() {
    arguments.clear();
}

bool QemuWrapper::Start() {
    if (IsRunning()) {
        std::cerr << "[QEMU-Wrapper] Error: VM is already running (PID: " << qemu_pid << ").\n";
        return false;
    }

    std::cout << "[QEMU-Wrapper] Starting VM...\n";
    
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[QEMU-Wrapper] Fork failed.\n";
        return false;
    }

    if (pid == 0) {
        // Child process
        std::vector<char*> args;
        args.push_back(const_cast<char*>(qemu_binary.c_str()));
        for (const auto& arg : arguments) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);

        execvp(qemu_binary.c_str(), args.data());
        
        // If execvp returns, it failed
        std::cerr << "[QEMU-Wrapper] Exec failed: " << strerror(errno) << "\n";
        exit(1);
    } else {
        // Parent process
        qemu_pid = pid;
        std::cout << "[QEMU-Wrapper] VM launched successfully. PID: " << qemu_pid << "\n";
        return true;
    }
}

bool QemuWrapper::Stop() {
    if (!IsRunning()) {
        return false;
    }

    std::cout << "[QEMU-Wrapper] Sending SIGTERM to PID: " << qemu_pid << "...\n";
    if (kill(qemu_pid, SIGTERM) == 0) {
        int status;
        waitpid(qemu_pid, &status, 0);
        std::cout << "[QEMU-Wrapper] VM stopped.\n";
        qemu_pid = -1;
        return true;
    }
    
    std::cerr << "[QEMU-Wrapper] Failed to stop VM.\n";
    return false;
}

bool QemuWrapper::IsRunning() const {
    if (qemu_pid <= 0) return false;
    
    // kill(pid, 0) checks if the process exists without sending a signal
    if (kill(qemu_pid, 0) == 0) {
        return true;
    }
    return false;
}

pid_t QemuWrapper::GetPid() const {
    return qemu_pid;
}

// Test main function
int main() {
    std::cout << "=== OS Project QEMU Wrapper Utility ===\n";
    QemuWrapper vm;
    
    // Add dummy arguments for a lightweight test instance
    vm.AddArgument("-m");
    vm.AddArgument("128");
    vm.AddArgument("-display");
    vm.AddArgument("none");
    
    if (vm.Start()) {
        sleep(2); // Let it run briefly
        vm.Stop();
    }

    std::cout << "[+] QEMU Wrapper testing completed.\n";
    return 0;
}

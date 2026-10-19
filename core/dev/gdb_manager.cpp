#include "gdb_manager.h"
#include <cstdlib>
#include <sstream>
#include <array>
#include <memory>

GdbManager::GdbManager(const std::string& target_exec) : executable_path(target_exec) {
    std::cout << "[GdbManager] Initialized for target executable: " << executable_path << "\n";
    if (!executable_path.empty()) {
        session_active = true;
    }
}

bool GdbManager::StartSession(const std::string& exec_path) {
    executable_path = exec_path;
    session_active = true;
    std::cout << "[GdbManager] Debug session started for " << executable_path << "\n";
    return true;
}

std::string GdbManager::RunCommand(const std::string& gdb_command) {
    if (!session_active) {
        return "ERROR: No active GDB session.";
    }

    std::string cmd = "gdb -q " + executable_path + " -ex \"" + gdb_command + "\" -ex \"quit\"";
    std::array<char, 256> buffer;
    std::string result = "";
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return "ERROR: failed to execute GDB command.";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string GdbManager::SetBreakpoint(const std::string& symbol_or_line) {
    return RunCommand("break " + symbol_or_line);
}

std::string GdbManager::Backtrace() {
    return RunCommand("backtrace");
}

bool GdbManager::EndSession() {
    session_active = false;
    std::cout << "[GdbManager] Debug session terminated.\n";
    return true;
}

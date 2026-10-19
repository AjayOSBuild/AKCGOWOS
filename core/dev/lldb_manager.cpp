#include "lldb_manager.h"
#include <cstdlib>
#include <sstream>
#include <array>
#include <memory>

LldbManager::LldbManager(const std::string& target_exec) : executable_path(target_exec) {
    std::cout << "[LldbManager] Initialized for target executable: " << executable_path << "\n";
    if (!executable_path.empty()) {
        session_active = true;
    }
}

bool LldbManager::StartSession(const std::string& exec_path) {
    executable_path = exec_path;
    session_active = true;
    std::cout << "[LldbManager] LLDB debug session started for " << executable_path << "\n";
    return true;
}

std::string LldbManager::RunCommand(const std::string& lldb_command) {
    if (!session_active) {
        return "ERROR: No active LLDB session.";
    }

    std::string cmd = "lldb -b -o \"" + lldb_command + "\" -- " + executable_path;
    std::array<char, 256> buffer;
    std::string result = "";
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return "ERROR: failed to execute LLDB command.";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string LldbManager::SetBreakpoint(const std::string& symbol_or_line) {
    return RunCommand("breakpoint set --name " + symbol_or_line);
}

std::string LldbManager::Backtrace() {
    return RunCommand("thread backtrace");
}

bool LldbManager::EndSession() {
    session_active = false;
    std::cout << "[LldbManager] LLDB debug session terminated.\n";
    return true;
}

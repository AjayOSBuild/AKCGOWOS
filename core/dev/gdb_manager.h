#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <cstdio>

class GdbManager {
private:
    std::string executable_path;
    bool session_active = false;

public:
    GdbManager(const std::string& target_exec = "");
    bool StartSession(const std::string& exec_path);
    std::string RunCommand(const std::string& gdb_command);
    std::string SetBreakpoint(const std::string& symbol_or_line);
    std::string Backtrace();
    bool EndSession();
};

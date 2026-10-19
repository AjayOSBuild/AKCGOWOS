#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

// --- Mock Win32 File I/O Types ---
typedef void* HANDLE;
typedef uint32_t DWORD;
typedef const char* LPCSTR;

#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)

// --- Path Redirection Core ---
class PathRedirector {
private:
    std::unordered_map<std::string, std::string> drive_mounts;

    void ToLower(std::string& str);
    void ReplaceSlashes(std::string& str);

public:
    PathRedirector();

    // Translates a Windows path (e.g., C:\Windows\System32) to a Linux VFS path
    std::string TranslatePath(const std::string& win_path);

    // Intercepted File I/O
    HANDLE CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
                      void* lpSecurityAttributes, DWORD dwCreationDisposition, 
                      DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
};

// --- Simulated Exported kernel32.dll APIs ---
namespace Kernel32API {
    HANDLE CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
                       void* lpSecurityAttributes, DWORD dwCreationDisposition, 
                       DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
}

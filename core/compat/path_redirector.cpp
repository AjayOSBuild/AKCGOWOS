#include "path_redirector.h"
#include <iostream>
#include <algorithm>

// --- Path Redirector Implementation ---
PathRedirector::PathRedirector() {
    std::cout << "[PathRedirector] Engine initialized. Establishing Windows drive mappings...\n";
    // Define standard compatibility sandbox prefixes
    drive_mounts["c:"] = "/root/.local/share/compat/drive_c";
    drive_mounts["z:"] = "/"; // Map Z: to Linux root (common Wine behavior)
}

void PathRedirector::ToLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void PathRedirector::ReplaceSlashes(std::string& str) {
    std::replace(str.begin(), str.end(), '\\', '/');
}

std::string PathRedirector::TranslatePath(const std::string& win_path) {
    if (win_path.empty()) return "";

    std::string translated = win_path;
    ReplaceSlashes(translated);

    // Check for drive letter (e.g., "C:/")
    if (translated.length() >= 2 && translated[1] == ':') {
        std::string drive_letter = translated.substr(0, 2);
        ToLower(drive_letter);

        auto it = drive_mounts.find(drive_letter);
        if (it != drive_mounts.end()) {
            std::string prefix = it->second;
            // Extract the rest of the path after the drive letter and optional slash
            std::string remainder = translated.substr(2);
            if (!remainder.empty() && remainder[0] == '/') {
                remainder = remainder.substr(1);
            }
            
            translated = prefix + "/" + remainder;
        }
    }

    // Windows paths are case-insensitive, but Linux is case-sensitive.
    // In full execution, this requires a directory scan to match case-insensitively.
    return translated;
}

HANDLE PathRedirector::CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
                                  void* lpSecurityAttributes, DWORD dwCreationDisposition, 
                                  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    
    std::string winPath = lpFileName ? lpFileName : "";
    std::cout << "[Win32:kernel32] CreateFileA intercepted for: " << winPath << "\n";
    
    std::string nativePath = TranslatePath(winPath);
    std::cout << "  -> Translated to native VFS path: " << nativePath << "\n";
    
    // In full execution, this routes to the StorageManager or native open() syscall
    std::cout << "  -> Mocking file handle creation.\n";
    return reinterpret_cast<HANDLE>(0x8899AABB);
}

// --- Simulated Exported kernel32.dll APIs ---
namespace Kernel32API {
    
    // Global instance of the redirector
    PathRedirector g_PathRedirector;

    HANDLE CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
                       void* lpSecurityAttributes, DWORD dwCreationDisposition, 
                       DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
        return g_PathRedirector.CreateFile(lpFileName, dwDesiredAccess, dwShareMode, 
                                           lpSecurityAttributes, dwCreationDisposition, 
                                           dwFlagsAndAttributes, hTemplateFile);
    }
}

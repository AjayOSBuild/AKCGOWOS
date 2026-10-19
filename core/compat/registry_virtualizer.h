#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

// --- Mock Win32 Registry Types ---
typedef void* HKEY;
typedef long LSTATUS;
typedef uint32_t REGSAM;
typedef uint32_t DWORD;
typedef const char* LPCSTR;
typedef uint8_t* LPBYTE;

#define ERROR_SUCCESS 0
#define ERROR_FILE_NOT_FOUND 2
#define HKEY_LOCAL_MACHINE ((HKEY)0x80000002)
#define HKEY_CURRENT_USER  ((HKEY)0x80000001)

// --- Registry Virtualization Core ---
class RegistryVirtualizer {
private:
    std::unordered_map<std::string, std::string> mock_registry_store;
    int next_handle = 0x1000;

public:
    RegistryVirtualizer();

    LSTATUS OpenKey(HKEY hKey, LPCSTR lpSubKey, HKEY* phkResult);
    LSTATUS QueryValue(HKEY hKey, LPCSTR lpValueName, DWORD* lpType, LPBYTE lpData, DWORD* lpcbData);
    LSTATUS CloseKey(HKEY hKey);
};

// --- Simulated Exported advapi32.dll APIs ---
namespace Advapi32API {
    LSTATUS RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, HKEY* phkResult);
    LSTATUS RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, DWORD* lpReserved, DWORD* lpType, LPBYTE lpData, DWORD* lpcbData);
    LSTATUS RegCloseKey(HKEY hKey);
}

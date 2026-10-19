#include "registry_virtualizer.h"
#include <iostream>
#include <cstring>

// --- Registry Virtualizer Implementation ---
RegistryVirtualizer::RegistryVirtualizer() {
    std::cout << "[Registry] Virtualization engine initialized. Mapping Windows hives to local flat files.\n";
    // Pre-seed some mock data for testing Windows applications
    mock_registry_store["Software\\Microsoft\\Windows\\CurrentVersion"] = "10.0.19045";
}

LSTATUS RegistryVirtualizer::OpenKey(HKEY hKey, LPCSTR lpSubKey, HKEY* phkResult) {
    std::cout << "[Registry] RegOpenKeyExA intercepted for subkey: " << (lpSubKey ? lpSubKey : "NULL") << "\n";
    
    // In full execution, this resolves the path against a TOML/JSON config map
    if (phkResult) {
        *phkResult = reinterpret_cast<HKEY>(++next_handle); // Return a mock handle
    }
    return ERROR_SUCCESS;
}

LSTATUS RegistryVirtualizer::QueryValue(HKEY hKey, LPCSTR lpValueName, DWORD* lpType, LPBYTE lpData, DWORD* lpcbData) {
    std::string valName = lpValueName ? lpValueName : "(Default)";
    std::cout << "[Registry] RegQueryValueExA intercepted for value: " << valName << "\n";
    
    // Simplified mock string return to satisfy standard legacy app queries
    if (lpData && lpcbData && *lpcbData > 0) {
        const char* mock_val = "MockData";
        size_t len = std::min(static_cast<size_t>(*lpcbData), strlen(mock_val) + 1);
        strncpy(reinterpret_cast<char*>(lpData), mock_val, len);
        *lpcbData = len;
    }
    return ERROR_SUCCESS;
}

LSTATUS RegistryVirtualizer::CloseKey(HKEY hKey) {
    std::cout << "[Registry] RegCloseKey intercepted for handle: " << hKey << "\n";
    return ERROR_SUCCESS;
}

// --- Simulated Exported Registry APIs ---
namespace Advapi32API {
    
    // Global instance of the virtualizer
    RegistryVirtualizer g_RegVirtualizer;

    LSTATUS RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, HKEY* phkResult) {
        return g_RegVirtualizer.OpenKey(hKey, lpSubKey, phkResult);
    }

    LSTATUS RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, DWORD* lpReserved, DWORD* lpType, LPBYTE lpData, DWORD* lpcbData) {
        return g_RegVirtualizer.QueryValue(hKey, lpValueName, lpType, lpData, lpcbData);
    }

    LSTATUS RegCloseKey(HKEY hKey) {
        return g_RegVirtualizer.CloseKey(hKey);
    }
}

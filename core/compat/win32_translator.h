#pragma once
#include <cstdint>
#include <string>

// Win32 Type Definitions
typedef void* HANDLE;
typedef void* LPVOID;
typedef uint32_t DWORD;
typedef uint32_t BOOL;
typedef const char* LPCSTR;

#define WIN32_TRUE 1
#define WIN32_FALSE 0

// Simulated Exported Win32 APIs
namespace Win32API {

    // kernel32.dll
    LPVOID VirtualAlloc(LPVOID lpAddress, size_t dwSize, DWORD flAllocationType, DWORD flProtect);
    BOOL VirtualFree(LPVOID lpAddress, size_t dwSize, DWORD dwFreeType);
    HANDLE CreateThread(LPVOID lpThreadAttributes, size_t dwStackSize, void* lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, DWORD* lpThreadId);
    
    // user32.dll
    HANDLE CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HANDLE hWndParent, HANDLE hMenu, HANDLE hInstance, LPVOID lpParam);
    BOOL DestroyWindow(HANDLE hWnd);

} // namespace Win32API

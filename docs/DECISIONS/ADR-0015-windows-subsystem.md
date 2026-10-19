# ADR 0015: Windows Compatibility Subsystem Architecture

## Status
Accepted

## Context
To support legacy Windows executables (`.exe`) and games without requiring a full virtual machine, Nexus OS requires a lightweight, user-space translation layer that bridges Win32/Win64 APIs and NT kernel system calls directly to native Linux system calls and hardware resources.

## Architecture

### 1. PE Loader & 32/64-bit Emulation
*   **Kernel Support:** Leverages `CONFIG_IA32_EMULATION=y` established during the kernel configuration phase to support 32-bit code execution on a 64-bit kernel.
*   **User-Space Loader:** A custom PE (Portable Executable) loader intercepts process creation requests from the `ApplicationManager`, mapping Windows PE sections into memory and resolving imports against translated system DLLs (`ntdll.dll`, `kernel32.dll`, `user32.dll`).

### 2. Graphics & DirectX Translation
*   **DirectX to Vulkan:** DirectX 9, 10, 11, and 12 translation pipelines (via DXVK and VKD3D concepts) map Direct3D command buffers directly to Vulkan API calls.
*   **Compositor Integration:** Translated graphical output is handed off as shared memory pixel buffers (`memfd`) to our custom Graphics Compositor, ensuring Windows apps render natively inside window frames managed by the Desktop Shell.

### 3. Filesystem & Registry Mapping
*   **Drive Letter Emulation:** Maps the root filesystem and user home directories to standard Windows drive conventions (e.g., `Z:\` maps to `/`, `C:\` maps to `~/.local/share/compat/drive_c/`).
*   **Registry Redirection:** Translates Windows Registry hives (`SYSTEM`, `USER`, `SOFTWARE`) into localized configuration files managed by the Configuration Service.

## Consequences
*   **Positive:** Enables high-performance execution of Windows applications with minimal CPU and memory overhead compared to full hardware virtualization.
*   **Negative:** Complex Win32 API edge cases, undocumented NT syscalls, and proprietary anti-cheat drivers may require specialized translation workarounds.

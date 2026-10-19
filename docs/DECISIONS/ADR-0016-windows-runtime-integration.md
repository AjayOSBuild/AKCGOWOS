# ADR 0016: Windows Subsystem Integration & Execution Pipeline

## Status
Accepted

## Context
Phase 9 established the individual translation blocks required to run Win32/Win64 binaries natively on Nexus OS. A unified runtime binary (`/bin/win_runner`) is required to bind these layers together and handle process execution requests from the `ApplicationManager`.

## Architecture

The execution pipeline flows through five primary subsystems:

1. **Path Redirection (`path_redirector.cpp`)**: Windows drive letters (`C:\`, `Z:\`) and path separators are translated into sandboxed Linux VFS directories (`~/.local/share/compat/drive_c`).
2. **Registry Virtualization (`registry_virtualizer.cpp`)**: HKEY lookups and modifications are intercepted and stored in isolated local maps, preventing modifications to system-wide Linux configs.
3. **PE Loader (`pe_loader.cpp`)**: Validates DOS (`MZ`) and NT (`PE`) headers, parses Section Tables, and allocates memory pages for `.text`, `.data`, and `.rdata`.
4. **API Translation (`win32_translator.cpp`)**: Bridges `kernel32.dll` and `user32.dll` functions directly to POSIX system calls (`mmap`, `std::thread`) and our Display Server IPC.
5. **DirectX-to-Vulkan Pipeline (`dx_translator.cpp`)**: Maps Direct3D 11/12 command buffers to Vulkan pipelines for GPU hardware acceleration.

## Consequences
* **Positive**: Fully native, out-of-process user-space translation layer with zero hypervisor overhead.
* **Negative**: Requires ongoing symbol stubs for specialized third-party DLL dependencies as more software is tested.

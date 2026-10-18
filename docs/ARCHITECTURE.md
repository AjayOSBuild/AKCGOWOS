# System Architecture Overview

This document outlines the high-level architecture of the operating system, structured into distinct, isolated layers from the bootloader up to the application runtimes. 

## 1. Boot & Hardware Foundation
*   **UEFI Bootloader:** Responsible for hardware initialization, secure boot verification, and loading the kernel into memory.
*   **Linux Kernel Foundation:** Provides the mature core for memory management, process scheduling, threading, and broad hardware/device driver support. 
*   **Initramfs:** The initial RAM filesystem that sets up the minimal environment, loads critical storage/filesystem modules, and mounts the actual root filesystem before handing control to the core service manager.

## 2. Core Services Layer (Ring 3)
*   **Service Manager:** The primary init system responsible for starting, monitoring, and restarting all system daemons.
*   **Device & Hardware Management:** Exposes hardware capabilities (e.g., Bluetooth, Wi-Fi, Audio) to user-space applications via secure APIs.
*   **Security & Permissions:** Enforces process isolation, sandbox boundaries, and file access policies.

## 3. Filesystem & Storage
*   **VFS Abstraction:** A unified virtual filesystem layer handling seamlessly integrated support for ext4, FAT32, exFAT, and NTFS.
*   **Storage Manager:** Manages mount/unmount operations, removable USB storage, and file indexing.

## 4. Graphics & Desktop Stack
*   **Compositor:** Hardware-accelerated (GPU) display server managing window surfaces and input events.
*   **Desktop Environment:** A Windows-inspired original UI featuring a taskbar, launcher menu, system tray, and file explorer.

## 5. Application Runtimes & Compatibility
*   **Native Linux Runtime:** Unrestricted execution of standard Linux ELF binaries within standard permissions.
*   **Windows Compatibility Layer:** Integrated environment handling EXE metadata, DLLs, and Windows API translation via progressive compatibility profiles.
*   **Android Runtime:** Sandboxed environment managing APK lifecycles, Android permissions, and seamless integration with the host desktop notification and storage systems.

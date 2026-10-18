# ADR 0008: UEFI Bootloader Architecture

## Status
Accepted

## Context
Phase 1 of the OS project required a custom UEFI bootloader capable of initializing virtual/physical hardware, parsing configurations, and safely transitioning to the Linux kernel foundation. The bootloader must support automated headless testing in QEMU while providing persistent crash diagnostics.

## Architecture & Sequence

The bootloader (`boot/main.c`) executes the following strict initialization sequence:

1.  **Hardware Diagnostics (COM1 & GOP):**
    *   Initializes the standard COM1 UART port (`0x3F8`) for headless serial logging.
    *   Queries the UEFI `EFI_GRAPHICS_OUTPUT_PROTOCOL` (GOP) to establish a graphical framebuffer and detect the screen resolution.
    *   Queries the firmware vendor and exact memory map size via `BS->GetMemoryMap`.

2.  **Filesystem & Logging:**
    *   Mounts the EFI System Partition (FAT32) using the `EFI_SIMPLE_FILE_SYSTEM_PROTOCOL`.
    *   Initializes a persistent file-based log (`EFI\BOOT\boot.log`). All subsequent system messages are routed simultaneously to the display console, serial port, and this log file.

3.  **Payload & Configuration Loading:**
    *   Locates the kernel binary (`bzImage`).
    *   Locates and parses the bootloader configuration (`boot.cfg`), extracting the `cmdline=` parameter to pass to the kernel environment.

4.  **Error Handling:**
    *   A centralized `Panic()` routine guarantees that if any critical resource (kernel, filesystem) is missing, the system flushes the persistent log, broadcasts a `[FATAL]` error, and halts the CPU, preventing undefined behavior.

## Consequences
*   **Positive:** The bootloader is entirely self-contained, heavily instrumented for QEMU automated CI testing, and avoids the complexity of third-party bootloaders like GRUB while remaining fully UEFI-compliant.
*   **Negative:** Because we are maintaining a custom bootloader, future requirements like Secure Boot signature verification or multi-boot selection menus must be manually implemented.

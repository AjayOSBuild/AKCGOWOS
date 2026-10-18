# Build Architecture

The OS project utilizes a hierarchical Make-based build system orchestrated by shell scripts. To prevent contamination from the host environment (Windows 11 / WSL), the build relies on a strictly isolated cross-compiler toolchain.

## Build Phases

1.  **Toolchain Preparation:** Verification of cross-compiler (e.g., `x86_64-elf-gcc`).
2.  **Bootloader (`boot/`):** Compiling the UEFI `.efi` executable.
3.  **Kernel (`kernel/`):** Compiling the core kernel into a `bzImage`.
4.  **User Space (`core/`, `services/`):** Compiling init and core services.
5.  **Initramfs:** Packaging user-space binaries into `initramfs.cpio.gz`.
6.  **Image Generation:** Creating the final bootable ISO/disk image.

## Commands

*   `./scripts/build.sh all` - Full build.
*   `./scripts/build.sh clean` - Clean build artifacts.
*   `make format` - Run clang-format on all C/C++ source files.

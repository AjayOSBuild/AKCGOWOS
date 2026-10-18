# ADR 0010: Virtual Filesystem (VFS) Abstraction

## Status
Accepted

## Context
To provide a unified filesystem experience across native Linux partitions (ext4), Windows compatibility drives (NTFS), and removable storage (FAT32, exFAT), the OS requires a Virtual Filesystem (VFS) abstraction layer in Ring 3. User-space applications must not need to know the underlying filesystem format to read or write files.

## Decision

We will implement a C++ VFS abstraction within the `StorageManager`.

### 1. Unified Interface
All filesystem implementations must inherit from a common `IVirtualFileSystem` interface providing standardized methods:
*   `Mount()` / `Unmount()`
*   `Open()` / `Close()`
*   `Read()` / `Write()`
*   `GetMetadata()`

### 2. Standardized Paths
The VFS will map physical blocks to standardized mount points:
*   `/` -> Primary ext4 root partition.
*   `/mnt/usb/` -> Removable FAT32/exFAT drives.
*   `~/.local/share/compat/drive_c/` -> Windows NTFS or compatibility mappings.

### 3. Error Handling
All VFS operations will return standardized error codes (e.g., `ERR_ACCESS_DENIED`, `ERR_FILE_NOT_FOUND`) rather than filesystem-specific hex codes, ensuring uniform error handling in the desktop UI.

## Consequences
*   **Positive:** Applications can use standard generic file I/O operations without caring about the storage medium. Adding new filesystems (like Btrfs) in the future only requires writing a new wrapper class.
*   **Negative:** Adds slight overhead to file operations as calls must be routed through virtual method tables before hitting the kernel syscalls.

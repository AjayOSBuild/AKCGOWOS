# ADR 0006: Subsystem Dependency Graph

## Status
Accepted

## Context
To prevent circular dependencies and ensure a stable boot sequence, we must define a strict initialization and build hierarchy for the operating system subsystems. Higher-level systems must never be initialized before their lower-level dependencies are fully ready.

## Decision

We define the following strict, acyclic dependency graph for both the build system and the runtime initialization sequence:

### Dependency Graph (Bottom-Up)

```text
[0] UEFI Bootloader
 └── [1] Linux Kernel
      ├── [2] Hardware Drivers (PCI, USB, NVMe)
      └── [2] Initramfs
           └── [3] Core Services (Service Manager, Device Manager, Logging)
                ├── [4] Filesystem (VFS, Mount Service)
                ├── [4] Networking (Ethernet, Wi-Fi, DHCP)
                └── [4] Security (Permission Manager)
                     └── [5] Graphics (Compositor, Window Surfaces)
                          ├── [6] Desktop UI (Taskbar, Launcher, System Tray)
                          │    └── [7] File Explorer
                          │    └── [7] System Utilities (Task Manager, Settings)
                          └── [6] Application Runtimes
                               ├── [7] Native Linux Applications
                               ├── [7] Windows Compatibility Layer (.exe)
                               └── [7] Android Runtime (.apk)
Initialization Rules
Level 0-2 (Ring 0 & Boot): Must be fully operational before user-space init begins.

Level 3 (Core Services): The Service Manager handles parallel initialization of Level 4 subsystems.

Level 5 (Graphics): Cannot start until permissions, basic networking, and device managers report successful hardware detection.

Level 6-7 (Desktop & Runtimes): Strictly depend on the Compositor. If the Compositor crashes, these subsystems are restarted.

Consequences
Positive: Prevents deadlocks during system startup. Enforces modularity and independent testability (e.g., testing Core Services headless without Graphics).

Negative: Strict layering requires careful API design to pass data down the stack (e.g., the UI requesting a network change must go through D-Bus to the Core Network Service, rather than directly touching network hardware).

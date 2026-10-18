# ADR 0005: QEMU Development Environment Configuration

## Status
Accepted

## Context
System-level development requires continuous execution of experimental code. To protect the host system (Windows 11) and facilitate the automated testing loop, all code must run in QEMU. Because the host system has 16 GB of RAM and is simultaneously running a local 14B parameter AI model (Qwen3-14B), the QEMU environment must be strictly resource-capped to prevent out-of-memory host crashes.

## Decision

We will standardize on a lightweight, heavily constrained QEMU virtual machine for the primary development and testing loop.

### 1. Virtual Hardware Constraints
*   **CPU:** 2 virtual cores (`-smp 2`). This is sufficient to test SMP (Symmetric Multiprocessing) concurrency in the kernel without overwhelming the host i7 processor.
*   **RAM:** 2 GB maximum (`-m 2G`). The OS must be optimized to boot and run its core services within this footprint.
*   **Storage:** 20 GB dynamically allocated `qcow2` virtual disk.
*   **Firmware:** OVMF (Open Virtual Machine Firmware) to support UEFI boot testing.

### 2. Execution Modes
*   **Headless / Automated Test Mode:** 
    Used by the continuous testing loop. No graphical window is spawned.
    `qemu-system-x86_64 -m 2G -smp 2 -bios OVMF.fd -drive file=disk.img,format=raw -nographic -serial file:build/qemu-serial.log`
*   **Interactive / Graphics Mode:** 
    Used when testing the graphics stack, compositor, and desktop UI.
    `qemu-system-x86_64 -m 2G -smp 2 -bios OVMF.fd -drive file=disk.img,format=raw -vga virtio -serial stdio`

### 3. Resource Monitor Integration
The autonomous testing loop must monitor host RAM. If host RAM usage exceeds 80%, new QEMU test runs must be paused to prevent the local AI agent context from being swapped to disk or crashing.

## Consequences
*   **Positive:** The host machine remains stable, allowing the local AI to function uninterrupted during continuous integration loops.
*   **Negative:** High-memory stress tests will require manual escalation or temporary overrides of the 2GB QEMU limit.

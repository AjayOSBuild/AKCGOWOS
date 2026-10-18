# ADR 0009: Kernel Foundation Architecture

## Status
Accepted

## Context
Phase 2 established the core operating system foundation by configuring the Linux kernel (`minimal_defconfig`). The kernel must be optimized for a highly responsive desktop environment while supporting advanced virtualization, compatibility layers (Windows/Android), and QEMU-based automated testing.

## Architectural Decisions

### 1. Scheduler and Synchronization
*   **Desktop Responsiveness:** Configured a fully preemptive kernel (`CONFIG_PREEMPT=y`) with a 1000Hz timer tick (`CONFIG_HZ_1000=y`) to ensure sub-millisecond latency for UI rendering and input events.
*   **Locking:** Enabled preemptive RCU (`CONFIG_PREEMPT_RCU=y`) and RCU priority boosting to prevent low-priority tasks from starving the compositor.

### 2. Memory Management
*   **Allocator:** Standardized on the SLUB allocator (`CONFIG_SLUB=y`) with page poisoning enabled during development to aggressively trap use-after-free bugs.
*   **Virtual Memory:** Enabled Kernel Samepage Merging (`CONFIG_KSM=y`) and Transparent Hugepages (`CONFIG_TRANSPARENT_HUGEPAGE=y`) to optimize the 2GB memory budget and deduplicate shared libraries across application sandboxes.

### 3. IPC and Compatibility
*   **System Calls:** Enabled 32-bit emulation (`CONFIG_IA32_EMULATION=y`) to support legacy Windows executables in Phase 9.
*   **IPC:** Enabled Unix Domain Sockets for D-Bus/compositor communication and Android Binder (`CONFIG_ANDROID_BINDER_IPC=y`) strictly for the Android compatibility layer.
*   **Security:** Enabled SECCOMP filtering to restrict system calls within untrusted application runtimes.

### 4. Diagnostics and Profiling
*   **Testing:** Integrated KUnit for in-kernel unit testing.
*   **Crash Handling:** Configured strict panic-on-oops behavior with a 5-second reboot timeout for automated CI loops. Enabled `pstore` (RAMOOPS) to preserve crash logs across warm reboots.
*   **Profiling:** Enabled `perf` events and eBPF (`CONFIG_BPF_SYSCALL=y`) for low-overhead performance tracing.

## Consequences
*   **Positive:** The kernel is perfectly tailored for our custom user space, balancing desktop latency with strict security boundaries and robust diagnostic capabilities.
*   **Negative:** Maintaining a custom `defconfig` requires manual review when upgrading to newer LTS kernel versions to ensure configurations remain valid.

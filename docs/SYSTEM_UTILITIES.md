# Nexus OS System Utilities Reference Guide

Phase 13 introduces the standalone system administration and diagnostics utility suite for Nexus OS. These applications provide deep inspection tools, recovery consoles, and storage management capabilities directly to administrators and power users.

## Utility Suite Overview

### 1. Advanced Task Manager (`/bin/sys_task_manager`)
*   **Telemetry:** Directly scans the virtual `/proc` filesystem to build a live inventory of running system processes.
*   **Metrics:** Parses process stats (`/proc/[pid]/stat`) to expose PIDs, process names, execution states, priority levels (`nice`), and resident set sizes (RSS memory consumption).
*   **Control:** Includes process termination hooks supporting graceful (`SIGTERM`) and forceful (`SIGKILL`) signal delivery.

### 2. Advanced Device Manager (`/bin/sys_device_manager`)
*   **Hardware Discovery:** Inspects the Linux virtual filesystem (`/sys/`) to map physical hardware components into structured categories.
*   **Inventory Domains:** Automatically catalogues PCI hardware buses, USB controllers, storage block devices, and active network interfaces alongside their explicit `sysfs` node paths.

### 3. Disk Management Utility (`/bin/sys_disk_manager`)
*   **Volume Inspection:** Parses `/proc/mounts` to identify all active storage volumes while intelligently filtering out kernel pseudo-filesystems (`proc`, `sysfs`, `devtmpfs`, etc.).
*   **Capacity Tracking:** Utilizes POSIX `statvfs` system primitives to query block limits, calculating total storage capacities, free space, and live usage percentages.

### 4. System Administration Terminal (`/bin/sys_terminal`)
*   **Recovery Console:** Provides an administrative command shell designed for system diagnostics and emergency troubleshooting.
*   **Built-in Diagnostics:** Supports commands such as `sysinfo` (kernel and memory overview), `services` (active core daemons check), `dmesg` (kernel ring buffer log inspection), and `reboot`.
*   **Theming:** Renders text in a classic phosphor-green admin console color scheme.

### 5. Comprehensive System Settings Control Panel (`/bin/sys_settings_app`)
*   **Centralized Configuration:** Organizes system preferences into a unified category-based sidebar interface (`Appearance`, `Power Management`, `Sound & Audio`, `User Accounts`).
*   **Persistence:** Directly modifies localized profile settings to coordinate visual themes and hardware profiles across the OS environment.

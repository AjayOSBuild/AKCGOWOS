# ADR 0001: Kernel and User-Space Boundaries

## Status
Accepted

## Context
To guarantee system stability, security, and portability, we must strictly define what operations belong in the Linux kernel foundation and what belongs in user space. Blurring these lines leads to kernel panics, security vulnerabilities, and unmaintainable code.

## Decision

We will enforce a strict Ring 0 (Kernel) and Ring 3 (User Space) separation.

### 1. Execution Privileges
*   **Kernel (Ring 0):** Only the core Linux kernel, essential hardware drivers, and critical interrupt handlers execute in Ring 0. 
*   **User Space (Ring 3):** All system services (including the Service Manager/init), window compositors, compatibility layers (Windows/Android), and user applications run in Ring 3.

### 2. Hardware Access
*   **Strictly Prohibited:** Direct I/O port access (`inb`/`outb`) or direct physical memory mapping (`/dev/mem`) from user-space applications is forbidden.
*   **Allowed:** User space must interact with hardware exclusively via exposed kernel APIs (e.g., standard Linux syscalls, `/dev` device nodes, `sysfs`, and `ioctl` interfaces).

### 3. Memory Isolation
*   Every user-space process operates in its own isolated virtual memory space.
*   The kernel maps user-space memory but user-space cannot map or read kernel memory. Null-pointer dereferences or segmentation faults in Ring 3 will cleanly terminate the offending process without impacting the core OS.

### 4. System Calls (Syscalls)
*   The boundary is crossed exclusively via the standard Linux syscall interface. Custom syscalls will be avoided unless absolutely necessary for the Windows/Android compatibility runtimes; standard POSIX/Linux APIs will be preferred.

## Consequences
*   **Positive:** The system will be highly resilient to user-space crashes (e.g., a crashing desktop UI will not panic the kernel).
*   **Negative:** High-performance hardware access (like direct GPU rendering) will require carefully managed DRM/KMS drivers in the kernel exposing safe interfaces to the user-space compositor, rather than direct user-space GPU drivers.

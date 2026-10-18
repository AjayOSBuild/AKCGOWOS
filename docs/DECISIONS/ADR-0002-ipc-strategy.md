# ADR 0002: Inter-Process Communication (IPC) Strategy

## Status
Accepted

## Context
A modern operating system requires processes to communicate securely and efficiently. Because our architecture includes native Linux services, a custom desktop UI, a Windows compatibility layer, and an Android runtime, we cannot rely on a single IPC mechanism. We need a strategy that balances standardization, performance, and compatibility.

## Decision

We will adopt a multi-tiered IPC strategy utilizing mature, existing Linux kernel and user-space mechanisms. We will avoid writing a custom IPC system from scratch to ensure reliability.

### 1. System Services & Desktop Messaging (Control Plane)
*   **Mechanism:** D-Bus.
*   **Usage:** Communication between the Service Manager, Device Manager, Desktop Environment (Taskbar, System Tray), and native applications.
*   **Reasoning:** D-Bus is the industry standard for Linux desktop IPC. It supports broadcast signals, method calls, and integrates well with security policies.

### 2. High-Performance Data Transfer (Data Plane)
*   **Mechanism:** Unix Domain Sockets (`AF_UNIX`) paired with Shared Memory (`memfd_create`).
*   **Usage:** The Graphics Compositor (passing pixel buffers), audio servers, and large file transfers in the File Explorer.
*   **Reasoning:** Passing file descriptors over Unix sockets allows zero-copy data transfer between isolated processes, which is critical for 60fps+ UI rendering.

### 3. Application Runtimes
*   **Android Compatibility:** We will enable and utilize the Linux kernel's **Binder** IPC mechanism exclusively for the Android runtime sandbox.
*   **Windows Compatibility:** Windows API IPC calls (e.g., Named Pipes, ALPC) will be translated by the compatibility layer into standard Unix Domain Sockets or POSIX shared memory.

### 4. Legacy / POSIX Standards
*   Standard POSIX pipes (`|`), signals, and message queues will remain fully supported for CLI utilities and standard native Linux binaries.

## Consequences
*   **Positive:** Utilizing D-Bus and Binder allows us to leverage massive amounts of existing open-source tooling and ensures seamless compatibility for Android apps.
*   **Negative:** D-Bus can be a bottleneck if misused for high-bandwidth data, requiring developers to strictly separate control messaging (D-Bus) from data payloads (Shared Memory).

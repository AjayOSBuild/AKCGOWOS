# Operating System Security Architecture

Security is a first-class subsystem in this project. This document defines the high-level security boundaries, privilege models, and threat mitigations required across the OS architecture.

## 1. Threat Model & Trust Boundaries

*   **Ring 0 (Kernel):** Fully trusted. Any vulnerability here compromises the entire system.
*   **Core Services (Ring 3 Daemons):** Highly trusted. Run with specific, limited capabilities (Linux capabilities) rather than full `root` access whenever possible.
*   **Native Applications:** Partially trusted. They run within the user's privilege scope but are subject to file-access limitations.
*   **Windows / Android Applications:** Untrusted. These runtimes execute arbitrary third-party code and must be strictly isolated.

## 2. Privilege & Permission Model

We reject the traditional model where every user application runs with full access to the user's entire home directory.

*   **Principle of Least Privilege:** System services must drop privileges immediately after binding required resources (like ports or device nodes).
*   **Administrative Access:** Elevation requires explicit, secure authentication (password or biometric via phone integration). No process can silently elevate to `root`.
*   **Permission Manager:** A centralized core service will broker all requests for sensitive hardware (Camera, Microphone, Location) and prompt the user for consent.

## 3. Application Sandboxing

### Native Applications
*   Native applications will utilize standard Linux namespaces (Mount, PID, Network, IPC) and cgroups to enforce resource limits and isolate dependencies.

### Windows Compatibility Layer
*   Windows processes (`.exe`) are confined to a specific virtualized directory structure (`~/.local/share/compat/drive_c/`).
*   They cannot access the native `/` filesystem, native system configurations, or native `/dev` nodes unless explicitly mapped by the user.

### Android Runtime
*   Android applications (`.apk`) are executed within a heavily restricted container using the native Linux Kernel Binder for IPC.
*   They are bound by the strict Android permission model, which the OS translates and enforces via the native Permission Manager.

## 4. Cryptography & Storage
*   **Secure Boot:** The UEFI bootloader must verify the digital signature of the kernel before execution to prevent rootkits.
*   **Encrypted Storage:** User data partitions must support block-level encryption (e.g., LUKS) to protect data at rest.
*   **Credentials:** Passwords, tokens, and encryption keys must be stored in a secure, encrypted credential storage service, never in plaintext configurations.

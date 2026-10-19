# Nexus OS Security Subsystem Reference Guide

Nexus OS implements a defense-in-depth security model designed to isolate applications, protect host system integrity, and enforce fine-grained user privacy controls. Phase 12 formalizes these mechanisms across the Ring 3 architecture.

## 1. Dynamic Permission Broker & ACLs (`core/security_policy.h`)
*   **Mechanism:** Sandboxed applications requesting access to sensitive resources (camera, microphone, location) must pass through the `PermissionManager`.
*   **Enforcement:** Incoming requests are evaluated against persistent Access Control Lists (ACLs). Unrecognized scopes trigger a user consent prompt to ensure explicit authorization before hardware access is granted.

## 2. Process Sandboxing (`core/sandbox_manager.h`)
*   **Isolation:** The `ApplicationManager` wraps application spawns in Linux namespaces:
    *   `CLONE_NEWPID`: Prevents host process inspection.
    *   `CLONE_NEWNS`: Confines filesystem visibility to authorized volumes and sandboxed prefixes.
    *   `CLONE_NEWNET` / `CLONE_NEWIPC` / `CLONE_NEWUTS`: Isolates network sockets, shared memory, and hostname spaces.
*   **Resource Throttling:** Control groups (cgroups v2) enforce hard memory ceilings (e.g., 512MB limits) and CPU scheduling weights to protect system responsiveness.

## 3. D-Bus IPC Security Policy (`core/dbus_policy_engine.h`)
*   **Message Filtering:** All inter-process communication routed over system D-Bus channels is inspected by the `DBusPolicyEngine`.
*   **Privilege Separation:** Unprivileged processes (UID >= 1000) are explicitly blocked from invoking privileged methods (such as `SystemEventService.Shutdown` or `StorageManager.Mount`), preventing containerized applications from escalating privileges.

## 4. Seccomp-BPF & AppArmor Confinement (`core/security_profile_generator.h`)
*   **Syscall Whitelisting:** Every containerized application is bound to a generated `seccomp-BPF` filter (`SCMP_ACT_KILL` default policy), restricting execution to essential system calls (`read`, `write`, `mmap`, etc.).
*   **Path Mediation:** AppArmor profiles restrict filesystem read/write access strictly to the application's designated prefix directory.

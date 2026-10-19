# ADR 0011: Graphics Display Server and Compositor Architecture

## Status
Accepted

## Context
A modern desktop operating system requires a dedicated display server and window compositor to manage graphical surfaces, handle input routing (mouse and keyboard), and deliver a smooth 60fps+ rendering loop via Direct Rendering Manager (DRM) and Kernel Mode Setting (KMS).

## Decision

We will implement a lightweight, custom C++ Compositor daemon (`/sbin/compositor`) running in Ring 3.

### 1. Rendering Backend
*   **Primary:** Direct Rendering Manager (DRM) / KMS via `/dev/dri/card0` for hardware acceleration on physical GPUs and VirtIO GPU in QEMU.
*   **Fallback:** Software framebuffer emulation utilizing the UEFI GOP framebuffer mapped memory.

### 2. Surface & Window Management
*   The Compositor maintains a z-ordered list of active window surfaces.
*   Applications (native, Windows compatibility, and Android runtimes) submit buffer handles via Unix Domain Sockets and Shared Memory (`memfd_create`), adhering to our multi-tiered IPC strategy (ADR-0002).

### 3. Input Integration
*   The Compositor polls the Linux input event nodes (`/dev/input/event*`) configured in Phase 3 to route keyboard and mouse events directly to the focused window surface.

## Consequences
*   **Positive:** Complete control over compositing effects, window decorations, and input latency without relying on monolithic legacy X11 servers.
*   **Negative:** Requires writing custom surface rendering logic, mitigated by targeting clean, modular Wayland-like protocol concepts.

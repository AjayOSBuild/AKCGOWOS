# ADR 0017: Android Compatibility Subsystem Architecture

## Status
Accepted

## Context
Nexus OS requires the ability to run Android applications (`.apk`) natively to provide users with a robust mobile app ecosystem. Instead of relying on a heavy virtual machine, we will utilize a container-based approach (inspired by Waydroid/Anbox) to run the Android Open Source Project (AOSP) system image directly on the host Linux kernel.

## Architecture

### 1. Containerization & Execution Environment
*   **Namespaces & Cgroups:** The `ApplicationManager` will spawn Android apps within isolated Linux namespaces (Mount, PID, IPC, Network, UTS) and enforce resource limits using cgroups.
*   **Rootfs:** A minimal AOSP system image (`system.img`, `vendor.img`) will be loop-mounted read-only inside the container's namespace to provide the Android framework runtime (ART, Zygote, System Server).

### 2. IPC & Kernel Features
*   **Binder:** Android's Binder IPC will be supported either via the native Linux kernel module (`CONFIG_ANDROID_BINDER_IPC=y`) or a user-space binder bridge mapping to D-Bus/Unix Domain Sockets.
*   **Ashmem:** Shared memory allocations from Android will be intercepted and mapped to POSIX `memfd_create` for zero-copy memory sharing with the host.

### 3. Hardware Abstraction Layer (HAL) Bridging
*   **Graphics (SurfaceFlinger):** The Android graphics HAL (gralloc) will be bridged to Nexus OS. Android window buffers will be exposed as `memfd` regions and submitted to the Nexus OS Display Server IPC (`/run/os/display.sock`).
*   **Input (EventHub):** Nexus OS Compositor will inject pointer and keyboard events directly into the Android container's virtual input devices.
*   **Audio & Sensors:** Bridged natively to the host's ALSA subsystem and Device Manager.

## Consequences
*   **Positive:** Near-native execution performance for Android apps with seamless window integration into the Nexus OS desktop.
*   **Negative:** Requires maintaining a customized AOSP system image and tightly coupling container lifecycle management to the OS Application Manager.

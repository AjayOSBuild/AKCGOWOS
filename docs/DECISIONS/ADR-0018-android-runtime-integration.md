# ADR 0018: Android Subsystem Integration & Execution Pipeline

## Status
Accepted

## Context
Phase 10 established the individual components required to execute Android applications (`.apk`) natively on Nexus OS. A unified runtime binary (`/bin/android_runner`) binds these layers together and acts as the secure entrypoint orchestrated by the Application Manager.

## Architecture

The Android execution pipeline flows through five primary subsystems:

1. **APK Parser (`apk_parser.cpp`)**: Extracts the APK archive and parses `AndroidManifest.xml` to resolve the package name, main activity, and requested permissions necessary for container setup.
2. **Container Lifecycle Manager (`android_container.cpp`)**: Spawns an isolated environment utilizing Linux namespaces (PID, Mount, UTS, IPC, Network) and cgroups, loop-mounting a minimal AOSP `system.img` read-only.
3. **Graphics HAL Bridge (`android_graphics_bridge.cpp`)**: Intercepts Android `ashmem`/`gralloc` buffer allocations and translates them to `memfd` blocks for zero-copy composition by the Nexus OS Display Server.
4. **Input Bridge (`android_input_bridge.cpp`)**: Translates host UI events into native `evdev` multi-touch and keycodes injected directly into the container's virtual input nodes.
5. **Binder IPC Bridge (`android_binder_bridge.cpp`)**: Hooks the `/dev/binder` ioctl interface to intercept Android framework calls and map them to native Nexus OS services over D-Bus.

## Consequences
* **Positive**: Allows Nexus OS to leverage the vast Android app ecosystem without full-system virtualization overhead, maintaining tight integration with the native window manager.
* **Negative**: Requires ongoing maintenance of the custom AOSP rootfs image and translation layers for new Android OS version upgrades.

# ADR 0013: Desktop Environment Architecture

## Status
Accepted

## Context
Phase 7 establishes the Desktop Shell (`/sbin/desktop_shell`), the primary Ring 3 graphical client that interacts with the Compositor. It is responsible for providing the user-facing workspace, application launching interfaces, system status tracking, and window decorations.

## Architecture

### 1. Compositor Client Integration
The Desktop Shell runs as an independent daemon. It connects to the Compositor via the Display Server IPC (`/run/os/display.sock`) to request a full-screen background surface and coordinate overlay rendering.

### 2. UI Component Hierarchy
The Shell orchestrates multiple sub-components utilizing a strict back-to-front rendering order (Painter's Algorithm):
1.  **Background Surface:** The root desktop wallpaper.
2.  **Compositor Surfaces:** Active application windows (managed by the Compositor but framed by the Shell).
3.  **Window Manager UI:** Renders window chrome (title bars, borders) and handles hit-testing for controls (minimize, maximize, close, drag).
4.  **Taskbar & System Tray:** Fixed overlay managing pinned shortcuts and live metrics (clock, battery, network).
5.  **Transient Overlays:** The Start Menu (application index/search) and Notification Center (alert queue).

## Consequences
*   **Positive:** Decoupling the Shell UI from the Compositor ensures that a crash in the Taskbar or Start Menu does not terminate active application windows or crash the display server.
*   **Negative:** Requires tightly synchronized 60fps rendering loops between the Compositor and the Shell to ensure window decorations track perfectly with moving application surfaces.

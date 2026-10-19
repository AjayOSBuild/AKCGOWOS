# ADR 0012: Graphics Pipeline & Input Routing

## Status
Accepted

## Context
Following the establishment of the foundational display server (ADR 0011), the compositor requires strict rules for handling overlapping window surfaces, routing user input securely, and receiving pixel buffers from unprivileged applications.

## Architecture

### 1. Window Management & Rendering (Painter's Algorithm)
The `CompositorEngine` manages a vector of `WindowSurface` objects. Rendering utilizes the Painter's Algorithm: surfaces are sorted by an ascending `z_index` and drawn back-to-front. 
*   **2D Abstraction:** A `Graphics2D` interface handles primitive shapes, borders, and UI backgrounds.
*   **Font Rendering:** A `FontRenderer` interface handles text rasterization for window chrome.

### 2. Secure Input Routing
To prevent keylogging and unintended UI interactions, the compositor acts as the sole reader of `/dev/input/event*`.
*   **Focus State:** Only one `WindowSurface` may hold the `is_focused` flag at any time.
*   **Keyboard:** Keystrokes are unconditionally routed exclusively to the focused surface.
*   **Pointer:** Mouse clicks and hovers undergo a strict bounding-box hit test. They are only dispatched if the absolute coordinates of the `HardwareCursor` intersect the spatial bounds (`x, y, width, height`) of the focused surface.

### 3. Display Server Protocol (IPC)
External client applications do not have direct access to the DRM/KMS backend.
*   Clients connect to the compositor via Unix Domain Sockets (`/run/os/display.sock`).
*   Clients invoke `CreateSurface` to negotiate a window.
*   Clients render their UI into a shared memory file descriptor (`memfd_create`) and pass the file descriptor to the compositor via `AttachBuffer`. This enables high-performance, zero-copy compositing.

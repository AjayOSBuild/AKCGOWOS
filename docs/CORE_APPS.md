# Nexus OS Core System Applications Reference

Phase 11 introduces the native application suite for Nexus OS. Built directly on top of our C++ UI Toolkit (`core/ui_toolkit.h`) and underlying system services, these applications provide users with essential daily productivity and system monitoring capabilities.

## Application Suite Overview

### 1. Terminal (`/bin/terminal`)
*   **Architecture:** Implements standard POSIX pseudo-terminal (PTY) routines (`posix_openpt`, `fork`, `execl`) to spawn and communicate with native `/bin/sh` shell sessions.
*   **Rendering:** Maintains a bounded scrollback buffer and utilizes the `FontRenderer` to draw monospaced text grids with real-time non-blocking input/output parsing.

### 2. Task Manager (`/bin/task_manager`)
*   **Telemetry:** Parses the Linux `/proc` filesystem (`/proc/meminfo`, `/proc/stat`) to calculate real-time system memory usage and track active process states (PID, name, memory consumption).
*   **Layout:** Renders dedicated system meter panels alongside a structured tabular process list.

### 3. System Settings (`/bin/settings`)
*   **Configuration Integration:** Acts as the user-facing control panel interfacing with the Core Configuration Service (Phase 4).
*   **Domains:** Divided into logical navigation categories: Display resolution scaling, Network connectivity, UI Personalization (themes), and System About details.

### 4. Calculator (`/bin/calculator`)
*   **State Machine:** Implements a robust arithmetic calculator handling numeric entry, chained operations, floating-point parsing, and safety guards against division-by-zero errors.
*   **Keypad:** Renders a standard 4x4 grid layout.

### 5. Text Editor (`/bin/text_editor`)
*   **Buffer Management:** Utilizes a multi-line document buffer (`std::vector<std::string>`) coupled with row/column cursor coordinate tracking.
*   **Filesystem Bridge:** Directly integrates with standard C++ file streams (`std::ifstream`/`std::ofstream`) to load, edit, and save plain text files on VFS-managed volumes.

### 6. Media Player (`/bin/media_player`)
*   **State Machine & Transport:** Manages playback states (PLAY, PAUSE, STOP, SEEK) and renders a dark-themed player interface featuring a progress scrubber and time counters.
*   **Backend Prep:** Scaffolded to bind audio streams to the ALSA backend and video frame buffers to the DRM graphics compositor.

### 7. Image Viewer (`/bin/image_viewer`)
*   **Inspection & Viewport:** Provides fluid zoom multipliers and pan coordinate offsets for photo inspection.
*   **Metadata Integration:** Interfaces with the `MetadataExtractor` and `ThumbnailGenerator` services to display image file properties and dimensions.

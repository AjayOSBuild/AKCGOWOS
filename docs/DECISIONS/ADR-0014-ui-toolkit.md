# ADR 0014: UI Toolkit Architecture

## Status
Accepted

## Context
Phase 8 introduces the native UI Toolkit for Nexus OS. To render the Desktop Shell (Taskbar, Start Menu, Notification Center) and future native applications, we require a modular, object-oriented 2D graphical user interface framework in C++.

## Architecture

### 1. Retained-Mode Widget Tree
The toolkit utilizes a retained-mode architecture (similar to Qt or GTK). 
*   **`Widget`**: The abstract base class representing a generic rectangular UI element with spatial properties (x, y, width, height).
*   **`Container`**: A specialized widget that manages a list of child widgets, responsible for layout routing and relative coordinate mapping.

### 2. Event Propagation
Input events (mouse clicks, key presses) captured by the Compositor and routed to the Desktop Shell are translated into `UIEvent` objects.
*   **Tunneling/Bubbling**: Events are passed to the root `Container`, which delegates them to the child widget that intersects the event coordinates. If a child does not consume the event, it bubbles back up to the parent container.

### 3. Rendering Context
Each `Widget` implements a virtual `Draw(Graphics2D& gfx)` method. The root container calls this recursively, drawing elements back-to-front.

## Consequences
*   **Positive:** Provides a clean, highly reusable API for rapidly building desktop applications and system menus.
*   **Negative:** Retained-mode GUIs consume more memory per element compared to immediate-mode GUIs (like Dear ImGui), requiring careful memory management of the widget tree.

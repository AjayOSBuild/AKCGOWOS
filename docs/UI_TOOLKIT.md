# Nexus OS Native UI Toolkit

The Nexus OS UI Toolkit provides a retained-mode, object-oriented C++ framework for rendering the Desktop Shell, taskbar, system menus, and native applications. It bridges the gap between the compositor's raw pixel buffers and interactive desktop elements.

## Widget Hierarchy

All visual elements inherit from the abstract `Widget` base class.

*   `Widget`: Provides fundamental properties (`x`, `y`, `width`, `height`, `visible`, `is_focused`) and virtual methods for drawing and event handling.
*   `Container`: A specialized `Widget` that owns a vector of child widgets. It handles drawing children back-to-front and routing events top-to-bottom.
    *   `LinearLayout`: Automatically positions children horizontally or vertically based on a defined spacing gap.
    *   `Menu`: A hidden-by-default vertical layout that implements click-away dismissal logic for context menus and drop-downs.

## Standard Components

The toolkit includes several pre-built interactive widgets:

*   `Button`: Executes a bound C++ `std::function` lambda when a `MOUSE_CLICK` event intersects its bounding box. Automatically manages hover and focus highlight colors.
*   `Label`: Renders static text via the `FontRenderer` abstraction.
*   `TextInput`: Captures `KEY_PRESS` events when focused, maintaining an internal string buffer and rendering a blinking text cursor.
*   `ImageBox`: Renders raster graphics (icons, wallpapers) via the `Graphics2D` engine.
*   `MenuItem`: A specialized `Button` optimized for vertical menu layouts, featuring edge-to-edge hover highlights.

## Event Propagation

Input events (mouse movements, clicks, and keystrokes) are captured by the `CompositorEngine` and dispatched as `UIEvent` structures.

1.  **Hit-Testing:** When a `MOUSE_CLICK` or `MOUSE_MOVE` occurs, the parent `Container` iterates through its children in reverse order (top-most first) and calls `HitTest(x, y)`.
2.  **Coordinate Translation:** If a child intersects the event, the coordinates are translated into the child's local space before `OnEvent()` is called.
3.  **Focus Management:** A successful click automatically updates the `is_focused` state across the widget tree, ensuring subsequent `KEY_PRESS` events are routed directly to the active input field.
4.  **Consumption:** If a widget acts upon an event, it sets `event.handled = true`, terminating the bubbling loop.

## Rendering Pipeline

Each rendering pass calls the `Draw(Graphics2D&, FontRenderer&)` method on the root container. The toolkit relies on two primary abstractions:
*   `Graphics2D`: Handles vector primitives (`FillRect`, `DrawRect`) and raster images (`DrawImage`).
*   `FontRenderer`: Handles text rasterization and glyph placement.

# OS Project Core APIs

This document defines the user-space D-Bus APIs provided by the Core Services (Ring 3) to the Desktop Environment and Application Runtimes.

## 1. Device Manager API

**Interface:** `os.core.DeviceManager`
**Object Path:** `/os/core/DeviceManager`

The Device Manager securely abstracts hardware interactions, preventing unprivileged applications from directly reading raw `/sys` or `/dev` nodes.

### Methods

*   `EnumerateDevices(String category) -> Array<Dict>`
    *   **Description:** Returns a list of attached devices matching a category (e.g., "storage", "input", "network", "audio").
    *   **Returns:** Array of dictionaries containing `DeviceId`, `Vendor`, `Product`, and `State`.

*   `GetDeviceProperties(String deviceId) -> Dict`
    *   **Description:** Retrieves detailed telemetry for a specific device (e.g., battery charge level, CPU temperature, disk size).

*   `RequestDeviceAccess(String deviceId, String reason) -> Boolean`
    *   **Description:** Prompts the Permission Manager to grant an application access to a specific hardware resource (like a webcam or microphone).

### Signals (Broadcasts)

*   `DeviceAdded(String deviceId, String category, Dict properties)`
    *   **Emitted when:** A new hardware device (e.g., USB drive, Bluetooth mouse) is hotplugged.
    
*   `DeviceRemoved(String deviceId)`
    *   **Emitted when:** A hardware device is disconnected.

*   `PowerStateChanged(String state, UInt8 batteryLevel)`
    *   **Emitted when:** The AC adapter is plugged/unplugged, or the battery drops below critical thresholds.

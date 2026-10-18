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

## 2. Core Service Standard API

**Interface:** `os.core.Service`
**Object Path:** `/os/core/[ServiceName]`

To fulfill Phase 4 requirements, every Ring 3 core daemon (e.g., DeviceManager, SessionManager) MUST implement this standard D-Bus interface to guarantee uniform orchestration by the master Service Manager.

### Methods

*   `Start() -> Boolean`
    *   **Description:** Initiates the service's primary event loop and claims required resources.
*   `Stop() -> Boolean`
    *   **Description:** Gracefully terminates the service, flushing pending operations to the Logging Service and safely closing sockets.
*   `Restart() -> Boolean`
    *   **Description:** Executes a `Stop()` followed immediately by a `Start()`.
*   `GetStatus() -> String`
    *   **Description:** Returns the current operational state (e.g., "RUNNING", "STOPPED", "FAILED", "IDLE").
*   `ReloadConfiguration() -> Boolean`
    *   **Description:** Instructs the service to dynamically re-read its settings from the Configuration Service without requiring a full restart.

### Signals (Broadcasts)

*   `StateChanged(String oldState, String newState)`
    *   **Emitted when:** The service transitions between operational states, notifying the master Service Manager.
*   `ErrorEncountered(Int32 code, String message)`
    *   **Emitted when:** A non-fatal internal error occurs, allowing the centralized Logging Service to instantly capture and structure the failure.

## 3. Storage Manager API

**Interface:** `os.core.StorageManager`
**Object Path:** `/os/core/StorageManager`

Abstracts raw filesystem mounts and block device manipulation.

### Methods

*   `Mount(String devicePath, String mountPoint, String fsType, String options) -> Boolean`
    *   **Description:** Securely requests the kernel to mount a block device. Enforces path traversal protections to ensure targets are within authorized `/mnt` or `~/.local` boundaries.
    *   **Returns:** True on success, False if the filesystem is unsupported or corrupted.

*   `Unmount(String mountPoint, Boolean force) -> Boolean`
    *   **Description:** Flushes pending disk operations and safely unmounts the specified path. If force is true, passes MNT_FORCE to forcefully detach unresponsive network or removable drives.
    *   **Returns:** True on success, False if the device is busy (EBUSY) or not found.

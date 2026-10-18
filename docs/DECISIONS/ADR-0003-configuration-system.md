# ADR 0003: Configuration System Architecture

## Status
Accepted

## Context
The operating system requires a robust, crash-resistant way to store settings for system services, user desktop preferences, and compatibility runtimes. We must avoid the pitfalls of a monolithic binary registry (which can corrupt the entire system if damaged) while still providing the required APIs for Windows applications to function.

## Decision

We will use a hierarchical, text-based configuration system for native components, isolated from compatibility runtimes.

### 1. Native System Services & Core (Ring 3 Daemons)
*   **Format:** TOML (Tom's Obvious, Minimal Language).
*   **Storage:** `/etc/os/` for global system configuration.
*   **Reasoning:** TOML is strongly typed, easily parsable by C/C++ libraries, and highly readable for system administrators debugging boot or service failures.

### 2. Native Desktop & User Preferences
*   **Format:** JSON.
*   **Storage:** `~/.config/os/` for per-user preferences.
*   **Reasoning:** JSON integrates seamlessly with web-technology-based UI widgets and modern desktop environments. 

### 3. Windows Compatibility Registry
*   **Strategy:** Windows applications expecting the Windows Registry will NOT touch the native TOML/JSON configurations.
*   **Storage:** The compatibility layer will maintain an isolated, virtualized registry database (likely utilizing Wine's registry structure or a similar open-source database) located inside the application sandbox (`~/.local/share/compat/registry/`).

### 4. Configuration Service API
*   To prevent file-locking issues, a centralized Configuration Service (via D-Bus) will handle read/write requests for dynamic user settings, ensuring atomic writes and emitting signals when preferences (like Dark Mode) change so the UI can update instantly.

## Consequences
*   **Positive:** A single corrupted configuration file will only break one service, not the entire OS. Text files remain easily editable from a recovery terminal.
*   **Negative:** Parsing text formats at boot incurs a slight performance penalty compared to memory-mapped binary databases, but this is mitigated by modern CPU speeds and the localized scope of the files.

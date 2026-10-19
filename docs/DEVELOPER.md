# Nexus OS Developer Environment Reference

Phase 14 establishes the native software development ecosystem for Nexus OS. The environment is designed to empower developers to build, debug, version-control, and package applications entirely within the operating system, eliminating the need for external cross-compilation environments.

## Subsystems & Tools

### 1. Compiler Manager (`/bin/compiler_manager`)
Orchestrates multi-language build toolchains.
*   **GCC & Clang Integration:** Configures optimization levels (`-O2`, `-Os`), warning profiles (`-Wall`, `-Werror`), and dynamically manages include and library paths.
*   **Clang Extensions:** Supports AddressSanitizer (`ASan`), UndefinedBehaviorSanitizer (`UBSan`), and automated `clang-tidy` static analysis.
*   **Rust & Cargo:** Natively wraps Cargo manifest parsing (`Cargo.toml`), managing target triples and release profiles.
*   **Python:** Manages script execution and automated virtual environment (`venv`) bootstrapping.

### 2. Version Control (`/bin/git_manager`)
Provides clean C++ IPC wrappers around the system `git` binary, allowing programmatic cloning, branch management, committing, and status parsing for IDEs and GUI clients.

### 3. Debugger Management (`/bin/gdb_manager` & `/bin/lldb_manager`)
Wraps the GNU Debugger (GDB) and LLVM Debugger (LLDB) via batch-mode pipes. These managers allow GUI applications and IDEs to programmatically set breakpoints, execute step commands, and extract thread backtraces.

### 4. SDK Manager (`/bin/sdk_manager`)
Automates the lifecycle of development kits (SDKs). It manages sysroot installations, compiler version switching, and architecture targets (e.g., `linux-sysroot-x86_64`, `wasm-wasi-sdk`).

### 5. Package Development Tools (`/bin/pkg_tools`)
Provides CLI and programmatic scaffolding for creating `.npk` (Nexus Package) applications. It generates standard `manifest.toml` files, lints directory structures, and performs archive compression.

### 6. Development Terminal (`/bin/dev_terminal`)
A customized terminal emulator that leverages `setenv` prior to forking the PTY. It automatically injects the active SDK paths, `C_INCLUDE_PATH`, `LD_LIBRARY_PATH`, and toolchain binaries into the shell context, providing an instant "Developer Command Prompt".

### 7. IDE Integration Broker (`/bin/ide_integration`)
Acts as the central IPC endpoint (`/run/os/ide_broker.sock`) for native and external Integrated Development Environments. It routes build requests, debugger sessions, and initializes Language Server Protocol (LSP) instances (like `clangd` or `rust-analyzer`) for intelligent code completion.

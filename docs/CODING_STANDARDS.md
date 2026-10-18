# Operating System Project Coding Standards

Consistency is critical for security, maintainability, and debugging. This document outlines the required coding standards for all contributions to the OS project.

## 1. C Programming (Kernel, Drivers, Bootloader)

We strictly adhere to the standard Linux Kernel coding style for all Ring 0 code.

*   **Indentation:** 8 characters, using tabs (not spaces).
*   **Line Length:** Maximum 80 characters.
*   **Brace Placement:** Opening brace on the same line as the statement (`if`, `for`, `while`). Opening brace on the next line for function definitions.
*   **Naming Conventions:** `snake_case` for variables, functions, and structs.
*   **Typedefs:** Avoid `typedef` for structs. Use `struct my_struct` directly to maintain transparency.
*   **Error Handling:** Functions should return an integer error code (`0` for success, negative for failure).
*   **Cleanup:** Use `goto` for centralized function cleanup and error unwinding to prevent memory leaks and lock deadlocks.

## 2. C++ Programming (User-Space Services, Desktop, Explorer)

User-space components will utilize C++ for better abstraction and memory safety tooling. We loosely follow the LLVM Coding Standards.

*   **Indentation:** 4 spaces.
*   **Line Length:** Maximum 100 characters.
*   **Naming Conventions:**
    *   Classes and Structs: `PascalCase`
    *   Methods and Functions: `camelCase`
    *   Variables: `camelCase`
    *   Constants/Macros: `UPPER_SNAKE_CASE`
*   **Memory Management:** Raw pointers (`*`) are strictly forbidden for ownership. Use `std::unique_ptr` and `std::shared_ptr`.
*   **Exceptions:** Avoid exceptions in critical core services to ensure deterministic execution. Use `std::expected` or similar result-wrapping patterns.

## 3. Shell Scripting (Build Scripts, Tooling)

*   **Interpreter:** Always use `#!/usr/bin/env bash`.
*   **Safety:** Always include `set -euo pipefail` at the top of every script.
*   **Variables:** Always quote variables (`"$MY_VAR"`) to prevent word splitting.
*   **Naming:** `UPPER_SNAKE_CASE` for global variables, `lower_snake_case` for local variables.

## 4. Automated Formatting

*   All C/C++ code must be formatted using `clang-format`.
*   A `.clang-format` file will be provided in the repository root in future architecture tasks.

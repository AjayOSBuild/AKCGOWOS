# Testing Architecture

Quality Assurance (QA) is mandatory. We do not accept "it compiles" as proof of correctness. Every subsystem (kernel, drivers, desktop, compatibility) must be verifiable through automated testing.

## 1. Test Categories

Every subsystem must implement the following test tiers:

*   **Smoke Tests:** Basic automated verification that a component builds and initializes without immediately crashing.
*   **Unit Tests:** Isolated execution of individual functions (e.g., memory allocators, string manipulation) using a lightweight C/C++ framework.
*   **Integration Tests:** Verification that interconnected subsystems (e.g., VFS and ext4 module) function correctly together in a headless environment.
*   **Failure Tests:** Intentional injection of faults (e.g., out-of-memory, simulated I/O errors) to verify graceful degradation and error recovery.
*   **Regression Tests:** Specific tests written to reproduce resolved bugs, ensuring they never return in future commits.

## 2. The QEMU Development Loop

System-level and kernel changes cannot be safely tested on the host machine. We enforce a strict 12-step automated QEMU testing loop for all system-level tasks:

1.  **Modify code:** Developer or AI agent implements changes.
2.  **Build:** Run the reproducible Make build system.
3.  **Update OS image:** Generate the bootable ISO or disk image.
4.  **Start QEMU:** Launch the virtual machine in headless/serial mode.
5.  **Capture logs:** Redirect QEMU serial output to a log file.
6.  **Run tests:** Execute automated scripts against the active QEMU instance.
7.  **Check exit status:** Verify the system reached the target milestone (e.g., init process started).
8.  **Collect crash logs:** Parse serial output for kernel panics or fatal errors.
9.  **Stop QEMU:** Safely terminate the virtual machine.
10. **Analyze:** Review test results and logs.
11. **Fix if required:** Adjust code based on failures.
12. **Repeat:** Maximum 3 automatic retries before escalating to human/expert review.

## 3. Host System Safety

*   Never automatically format a physical disk.
*   Never run experimental kernel code directly on the host hardware.
*   All disk operations during testing must target QEMU virtual disks.

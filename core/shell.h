#pragma once
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>

static inline void run_shell(void) {
    char cmd[128];
    printf("NexusOS Core Shell (v0.1.0-alpha)\nType 'help' for available commands.\n\n");

    while (1) {
        printf("nexus@localhost:~$ ");
        fflush(stdout);

        if (!fgets(cmd, sizeof(cmd), stdin)) break;
        cmd[strcspn(cmd, "\r\n")] = 0;

        if (strlen(cmd) == 0) continue;

        if (strcmp(cmd, "help") == 0) {
            printf("Built-in commands:\n");
            printf("  help     - Show command list\n");
            printf("  uname    - Display OS and kernel information\n");
            printf("  meminfo  - Display system RAM usage\n");
            printf("  update   - Check for and stage OS feature updates\n");
            printf("  clear    - Clear terminal screen\n");
            printf("  reboot   - Restart system\n");
            printf("  poweroff - Halt and power down system\n");
        } else if (strcmp(cmd, "uname") == 0) {
            printf("NexusOS 0.1.0-alpha x86_64 Linux 6.6 LTS (Hardened)\n");
        } else if (strcmp(cmd, "meminfo") == 0) {
            FILE *f = fopen("/proc/meminfo", "r");
            if (f) {
                char line[128];
                for (int i = 0; i < 3 && fgets(line, sizeof(line), f); i++) printf("  %s", line);
                fclose(f);
            } else {
                printf("Error: /proc not mounted\n");
            }
        } else if (strncmp(cmd, "update", 6) == 0) {
            printf("[UPDATER] Checking update channel 'stable'...\n");
            printf("[UPDATER] System is running baseline build ARCH-001.\n");
            printf("[UPDATER] No pending payloads. Module slot: READY for updates.\n");
        } else if (strcmp(cmd, "clear") == 0) {
            printf("\033[H\033[J");
        } else if (strcmp(cmd, "reboot") == 0) {
            printf("Rebooting system...\n");
            sync();
            reboot(LINUX_REBOOT_CMD_RESTART);
        } else if (strcmp(cmd, "poweroff") == 0) {
            printf("Powering off system...\n");
            sync();
            reboot(LINUX_REBOOT_CMD_POWER_OFF);
        } else {
            printf("nexus: command not found: %s\n", cmd);
        }
    }
}

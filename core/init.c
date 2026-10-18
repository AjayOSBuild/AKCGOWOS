#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

void setup_filesystems() {
    printf("[*] Mounting virtual filesystems...\n");
    mount("none", "/proc", "proc", 0, "");
    mount("none", "/sys", "sysfs", 0, "");
    mount("none", "/dev", "devtmpfs", 0, "");
}

int main() {
    // PID 1 identifying output
    printf("\n=== OS Project Core Init (PID 1) ===\n");
    
    if (getpid() != 1) {
        printf("[!] Warning: Not running as PID 1.\n");
    }

    setup_filesystems();

    printf("[+] Core Services initialized. Entering master process loop...\n");

    // Master loop to reap zombie processes (critical for PID 1)
    sigset_t set;
    int status;
    
    sigfillset(&set);
    sigprocmask(SIG_BLOCK, &set, NULL);

    while (1) {
        // Wait for any child process to change state
        pid_t pid = waitpid(-1, &status, 0);
        if (pid > 0) {
            printf("[*] Reaped child process %d\n", pid);
        }
    }

    return 0;
}

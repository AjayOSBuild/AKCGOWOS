#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>

// Background thread function to validate scheduler concurrency
void* system_heartbeat(void* arg) {
    (void)arg;
    while (1) {
        printf("[Diag] Init (PID 1) Thread Heartbeat: System is stable.\n");
        sleep(5);
    }
    return NULL;
}

void setup_filesystems() {
    printf("[*] Mounting virtual filesystems...\n");
    mount("none", "/proc", "proc", 0, "");
    mount("none", "/sys", "sysfs", 0, "");
    mount("none", "/dev", "devtmpfs", 0, "");
}

int main() {
    printf("\n=== OS Project Core Init (PID 1) ===\n");
    
    if (getpid() != 1) {
        printf("[!] Warning: Not running as PID 1.\n");
    }

    setup_filesystems();

    printf("[*] Initializing system thread pool...\n");
    pthread_t heartbeat_thread;
    if (pthread_create(&heartbeat_thread, NULL, system_heartbeat, NULL) != 0) {
        printf("[FATAL] Failed to create system heartbeat thread. Check CONFIG_FUTEX.\n");
        exit(1);
    }

    printf("[+] Core Services initialized. Entering master process loop...\n");

    // Master loop to reap zombie processes
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

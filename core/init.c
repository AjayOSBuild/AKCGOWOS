#include <stdio.h>
#include <unistd.h>
#include <sys/mount.h>
#include "../boot/splash.h"
#include "shell.h"

int main() {
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);
    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);

    show_windows_splash();
    run_shell();

    return 0;
}

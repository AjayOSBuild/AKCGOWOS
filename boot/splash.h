#pragma once
#include <unistd.h>
#include <stdio.h>

static inline void show_windows_splash(void) {
    const char *frames[] = {
        "        [  N E X U S   O S  ]\n\n             *  .  .  .  .",
        "        [  N E X U S   O S  ]\n\n             .  *  .  .  .",
        "        [  N E X U S   O S  ]\n\n             .  .  *  .  .",
        "        [  N E X U S   O S  ]\n\n             .  .  .  *  .",
        "        [  N E X U S   O S  ]\n\n             .  .  .  .  *"
    };
    for (int i = 0; i < 15; i++) {
        printf("\033[H\033[J\n\n\n\n\n\n%s\n", frames[i % 5]);
        fflush(stdout);
        usleep(120000);
    }
    printf("\033[H\033[J");
}

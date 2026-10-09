#include "kernel/shell/shell.h"
#include "lib/std/stdio.h"
#include "lib/std/time.h"

// sleep
int scmd_sleep(int argc, char ** argv) {
    unsigned int time_seconds = 20;

    printf("Sleeping for %d seconds.\n", time_seconds);
    sleep(time_seconds*1000);
    printf("Done.\n");
    return SS_OK;
}

void scmd_register_sleep() {
    register_cmd("sleep", 1, &scmd_sleep);
}

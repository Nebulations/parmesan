#include "kernel/shell/shell.h"
#include "lib/std/stdio.h"
#include "lib/std/time.h"

// uptime
int scmd_uptime(int argc, char ** argv) {
    printf("Current uptime: %d ms\n", get_ticks());
    return SS_OK;
}

void scmd_register_uptime() {
    register_cmd("uptime", 1, &scmd_uptime);
}

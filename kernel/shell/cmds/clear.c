#include "kernel/shell/shell.h"
#include "lib/std/stdio.h"

int scmd_clear(int argc, char ** argv) {
    clear_screen();
    return SS_OK;
}

void scmd_register_clear() {
    register_cmd("clear", 1, &scmd_clear);
}

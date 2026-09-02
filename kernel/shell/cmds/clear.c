#include "kernel/shell/shell.h"
#include "lib/std/stdio.h"

#define SS_OK 0
#define SS_EXIT 1
#define SS_BAD_CMD 2
#define SS_INVALID_ARGS 3

int scmd_clear(int argc, char ** argv) {
    clear_screen();
    return SS_OK;
}

void scmd_register_clear() {
    register_cmd("clear", 1, &scmd_clear);
}

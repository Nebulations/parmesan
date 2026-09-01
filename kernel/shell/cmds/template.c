#include "kernel/shell/shell.h"
#include "drivers/display/vga/terminal.h"

#define SS_OK 0
#define SS_EXIT 1
#define SS_BAD_CMD 2
#define SS_INVALID_ARGS 3

// example
int scmd_example(int argc, char ** argv) {
    
    return SS_OK;
}

void scmd_register_example() {
    register_cmd("example", 1, &scmd_example);
}

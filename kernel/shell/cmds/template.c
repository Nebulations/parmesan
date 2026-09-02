#include "kernel/shell/shell.h"
#include "drivers/display/vga/terminal.h"

// example
int scmd_example(int argc, char ** argv) {
    
    return SS_OK;
}

void scmd_register_example() {
    register_cmd("example", 1, &scmd_example);
}

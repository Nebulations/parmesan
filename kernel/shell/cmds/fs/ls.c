#include "kernel/shell/shell.h"
#include "drivers/display/vga/terminal.h"

#include "fs/vfs/vfs.h"

int scmd_ls(int argc, char ** argv) {
    

    return SS_OK;
}

void scmd_register_ls() {
    register_cmd("ls", 1, &scmd_ls);
}

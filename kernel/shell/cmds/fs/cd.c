#include "kernel/shell/shell.h"

#include "lib/std/stdio.h"

#include "fs/vfs/vfs.h"

int scmd_cd(int argc, char ** argv) {
    if (argc == 0) {
        printf("No directory provided.\n");
        return SS_NONE;
    }

    // Directory provided from the first argument.
    char * directory = argv[0];

    vfs_setpath(directory);

    return SS_OK;
}

void scmd_register_cd() {
    register_cmd("cd", 1, &scmd_cd);
}

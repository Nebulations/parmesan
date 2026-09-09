#include "kernel/shell/shell.h"
#include "lib/std/stdio.h"

#include "fs/vfs/vfs.h"

int scmd_whereami(int argc, char ** argv) {
    printf("Current work directory: %s\n", vfs_path());
    return SS_OK;
}

void scmd_register_whereami() {
    register_cmd("whereami", 1, &scmd_whereami);
}

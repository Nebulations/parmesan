#include "kernel/shell/shell.h"
#include "kernel/memory/memory.h"

#include "lib/std/stdio.h"

// res
int scmd_res(int argc, char ** argv) {
    uint64_t total_memory = get_total_memory();
    uint64_t free_memory = get_free_memory();

    uint64_t used_memory = total_memory - free_memory;

    print("System memory usage:\n");
    printf("Total memory: %d\nFree memory: %d\nUsed memory: %d\n", total_memory, free_memory, used_memory);

    return SS_OK;
}

void scmd_register_res() {
    register_cmd("res", 1, &scmd_res);
}

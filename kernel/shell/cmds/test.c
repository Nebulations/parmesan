#include "kernel/shell/shell.h"
#include "drivers/display/vga/terminal.h"
#include "kernel/memory/memory.h"
#include "lib/string.h"

#define SS_OK 0
#define SS_EXIT 1
#define SS_BAD_CMD 2
#define SS_INVALID_ARGS 3

int scmd_test(int argc, char ** argv) {
    if (argc == 0) {
        return SS_INVALID_ARGS;
    }

    if (str_equals(argv[0], "mem")) {
        int* a = (int*) malloc(sizeof(int));
        *a = 123;

        print("a = ");
        print_hex((uint32_t) a);
        println("");

        free(a);

        int* b = (int*) malloc(sizeof(int));
        *b = 123;

        print("b = ");
        print_hex((uint32_t) b);
        println("");

        free(b);

        return SS_OK;
    }
}

void scmd_register_test() {
    register_cmd("test", 1, &scmd_test);
}

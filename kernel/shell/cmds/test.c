#include "kernel/shell/shell.h"
#include "kernel/memory/memory.h"
#include "lib/string.h"

#include "lib/std/stdio.h"

int scmd_test(int argc, char ** argv) {
    if (argc == 0) {
        return SS_INVALID_ARGS;
    }

    if (str_equals(argv[0], "mem")) {
        int* a = (int*) malloc(sizeof(int));
        *a = 123;

        printf("a = %d\n", a);
        free(a);

        int* b = (int*) malloc(sizeof(int));
        *b = 123;

        printf("b = %d\n", b);

        free(b);

        return SS_OK;
    }

    if (str_equals(argv[0], "print")) {
        print("Standard print");
        printf("\nprintf: %s |||| %d", "Test 1", 1234);
        return SS_OK;
    }

    return SS_INVALID_ARGS;
}

void scmd_register_test() {
    register_cmd("test", 1, &scmd_test);
}

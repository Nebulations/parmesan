#include "kernel/shell/shell.h"
#include "kernel/memory/memory.h"
#include "lib/string.h"

#include "lib/std/stdio.h"
#include "lib/std/time.h"

#include "drivers/timings/pit/pit.h"

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
        printf("printf: %s |||| %d", "Test 1", 1234);
        return SS_OK;
    }

    if (str_equals(argv[0], "time")) {
        printf("Current ticks: %d\n", pit_get_ticks());
        return SS_OK;
    }

    if (str_equals(argv[0], "sleep")) {
        print("Waiting for 3s\n");
        uint32_t start = pit_get_ticks();

        sleep(3000);

        uint32_t end = pit_get_ticks();

        printf("Start: %d\n", start);
        printf("End: %d\n", end);
        printf("Elapsed ticks: %d\n", end - start);

        return SS_OK;
    }

    if (str_equals(argv[0], "halt")) {
        print("System halted.\n");

        __asm__ volatile ("sti");
        __asm__ volatile ("hlt");

        print("System unhalted.\n");
        return SS_OK;
    }

    return SS_INVALID_ARGS;
}

void scmd_register_test() {
    register_cmd("test", 1, &scmd_test);
}

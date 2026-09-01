#include "../shell.h"
#include "../../terminal.h"

#define SS_OK 0
#define SS_EXIT 1
#define SS_BAD_CMD 2
#define SS_INVALID_ARGS 3

int scmd_echo(int argc, char ** argv) {
    for (int i = 0; i < argc; i++) {
        char* arg = argv[i];

        while (*arg != '\0') {
            print_char(*arg);
            arg++;
        }
    }

    return SS_OK;
}

void scmd_register_echo() {
    register_cmd("echo", 1, &scmd_echo);
}

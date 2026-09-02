#include "kernel/shell/shell.h"
#include "drivers/display/vga/terminal.h"

int scmd_echo(int argc, char ** argv) {
    for (int i = 0; i < argc; i++) {
        char* arg = argv[i];

        while (*arg != '\0') {
            print_char(*arg);
            arg++;
        }

        // Add a space so arguments are seperated.
        print(" ");
    }

    return SS_OK;
}

void scmd_register_echo() {
    register_cmd("echo", 1, &scmd_echo);
}

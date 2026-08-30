#include "terminal.h"
#include "util/string.h"
#include "memory.h"

#define SS_OK 0
#define SS_EXIT 1
#define SS_BAD_CMD 2

int shell_process(char * input);
void _help_command();

int shell_process(char * input) {
    if (str_equals(input, "clear")) {
        undraw_cursor();
        clear_screen();
        set_cursor(0, 0);
        draw_cursor();
        return SS_OK;
    }

    if (str_equals(input, "exit")) {
        return SS_EXIT;
    }

    if (str_equals(input, "help")) {
        _help_command();
        return SS_OK;
    }

    if (str_equals(input, "hello")) {
        println("Hello, World!");
        return SS_OK;
    }

    if (str_starts_with(input, "echo ") == 1) {
        // Shift the pointer so it starts further down the line than "echo "
        input+=5;

        println(input);

        return SS_OK;
    }

    if (str_equals(input, "test")) {
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

        return SS_OK;
    }

    return SS_BAD_CMD;
}

void _help_command() {
    println("Help (1/1)");
    println("| hello - Outputs 'Hello World!'");
    println("| clear - Clear the terminal");
    println("| echo <text> - Echo text to the terminal");
    println("| help - Display this help menu");
    println("| exit - Exit the shell");
}
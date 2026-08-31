#include "terminal.h"
#include "util/string.h"
#include "memory.h"

#define SS_OK 0
#define SS_EXIT 1
#define SS_BAD_CMD 2

// Allocate a 128 byte buffer for user input.
static char inputBuffer[128];
static int inputBufferLength = 0;

int shell_process(char * input);
void process_key(char c);

void _help_command();

void process_key(char c) {
    if (c == 0) {
        return;
    }

    // Check if the char is 'enter'
    if (c == '\n') {
        undraw_cursor();
        set_cursor(0, get_cursor_y()+1);

        inputBuffer[inputBufferLength] = '\0';

        int res = shell_process(inputBuffer);
        // Reset buffer before reprocessing user input
        inputBufferLength = 0;

        if (res != 0) {
            if (res == 1) {
                return;
            }
            println("Error!");
        }

        println("> ");
        draw_cursor();

        return;
    }

    // Check if we're backspacing/deleting chars
    if (c == '\b') {
        // We already deleted everything so we do nothing.
        if (inputBufferLength == 0) {
            return;
        }

        undraw_cursor();
        set_cursor(get_cursor_x()-1, get_cursor_y());
        inputBufferLength--;
        draw_cursor();
        return;
    }

    // Print the character to the terminal.
    print_char(c);
    draw_cursor();

    inputBuffer[inputBufferLength] = c;
    inputBufferLength++;
}

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

    if (str_equals(input, "test memory")) {
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
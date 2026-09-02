#include<stdarg.h>
#include<stdint.h>

#include "drivers/display/vga/terminal.h"

#include "lib/string.h"

/**
 * Clears the screen.
 */
void clear_screen() {
    clear_terminal();
}

/**
 * Print out a message to the std output.
 * 
 * @param message The message to output
 */
void print(char * message) {
    for (int i = 0; message[i] != '\0'; i++) {
        char c = message[i];
        if (i >= SCREEN_X) {
            set_cursor(0, get_cursor_y() + 1);
        }

        if (c == '\n') {
            set_cursor(0, get_cursor_y() + 1);
            continue;
        }

        write_char(message[i], get_cursor_x(), get_cursor_y());
        set_cursor(get_cursor_x() + 1, get_cursor_y());
    }
}

/**
 * Prints out a message to the standard output. You can also
 * use classic C-style modifiers, such as %s or %d to format
 * content within the message.
 * 
 * @param message The message to output, with the formatting inside
 */
void printf(const char * message, ...) {
    va_list args;

    va_start(args, message);

    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\n') {
            set_cursor(0, get_cursor_y()+1);
            continue;
        }

        char format[] = {message[i], message[i+1], '\0'};

        // string args
        if (str_equals(format, "%s")) {
            char * string = va_arg(args, char *);
            print(string);
            i++;
            continue;
        }

        // int args
        if (str_equals(format, "%d")) {
            int num = va_arg(args, int);

            print_int(num);

            i++;
            continue;
        }

        print_char(message[i]);
    }

    va_end(args);
}
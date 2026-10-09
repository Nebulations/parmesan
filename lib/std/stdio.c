#include<stdarg.h>
#include<stdint.h>

#include "drivers/display/vga/terminal.h"

#include "lib/string.h"

/**
 * Clears the screen and resets the cursor's position to 0, 0
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
        // New lines
        if (message[i] == '\n') {
            set_cursor(0, get_cursor_y()+1);
            continue;
        }

        // Null terminated string
        if (message[i] == '\0') {
            printf("flushing here...");
            va_end(args);
            return;
        }

        char format[] = {message[i], message[i+1], '\0'};

        // string args
        if (str_equals(format, "%s")) {
            print(va_arg(args, char *));
            i++;
            continue;
        }

        // int args
        if (str_equals(format, "%d")) {
            print_int(va_arg(args, int));

            i++;
            continue;
        }

        // char args
        if (str_equals(format, "%c")) {
            print_char(va_arg(args, char));
            i++;
            continue;
        }

        print_char(message[i]);
    }

    va_end(args);
}
#include "drivers/display/vga/terminal.h"
#include "lib/array.h"
#include "lib/string.h"
#include "kernel/memory/memory.h"
#include "cmds.h"

#define SS_OK 0
#define SS_EXIT 1
#define SS_BAD_CMD 2
#define SS_INVALID_ARGS 3

// Allocate a 128 byte buffer for user input.
static char inputBuffer[128];
static int inputBufferLength = 0;

// Stuff related to the shell command API
typedef int (*command_handler_t)(int argc, char ** argv);

typedef struct {
    char * name;
    uint8_t argc;
    command_handler_t handler;
} shell_command_t;

shell_command_t commands[10];
uint8_t command_count = 0;

void shell_register(shell_command_t * command) {
    commands[command_count] = *command;
    command_count++;
}

/**
 * Register a command to the shell API.
 * 
 * @param name The name of the command
 * @param argc How many arguments the command has, EXCLUDING the command's name.
 * @param command The command to register
 */
void register_cmd(char * name, int argc, command_handler_t command) {
    shell_command_t cmd = {name, argc+1, command};
    shell_register(&cmd);
}

void shell_init() {
    scmds_init();
}

void process_keyboard_input(char c, uint8_t code, uint8_t keyboard_map_len);

/**
 * Process the user's input
 * @param c The character mapped to the currently used keyboard.
 * @param code The keyboard code.
 * @param keyboard_map_len The length of the keyboard map used. 
 */
void process_keyboard_input(char c, uint8_t code, uint8_t keyboard_map_len) {
    int released = code & 0x80;

    // Validate the key is in range and not outside the array
    // so we don't access random memory addresses.
    if (code >= keyboard_map_len) {
        println("OUT OF BOUNDS");
        return;
    }

    if (c == 0) {
        return;
    }

    // Check if the char is 'enter'
    if (c == '\n') {
        undraw_cursor();
        set_cursor(0, get_cursor_y()+1);

        inputBuffer[inputBufferLength] = '\0';

        // The name of the command
        char command_name[32];
        // What the current argument is
        char arguments[256];
        // Max 16 arguments in the command.
        char* argv[16];
        // How many arguments are there.
        int argc = 0;
        // Check if the command name was found.
        int name_found = 0;

        // Assume that the command is wrong by default.
        int res = SS_BAD_CMD;

        int argument_index = 0;
        int argument_started = 0;
        int starting_string = 0;

        for (int i = 0; i < inputBufferLength; i++) {
            char current_char = inputBuffer[i];

            // Check if we're starting or ending a string.
            if (current_char == '"') {
                starting_string = starting_string == 0 ? 1 : 0;
                continue;
            }

            if (current_char == ' ' && starting_string == 0) {
                if (name_found == 0) {
                    name_found = 1;
                    command_name[i] = '\0';
                    continue;
                }

                // End current argument
                arguments[argument_index] = '\0';
                argument_index++;

                argument_started = 0;
                continue;
            }

            if (name_found == 0) {
                command_name[i] = current_char;
                continue;
            }

            // Starting a new argument
            if (argument_started == 0) {
                arguments[argument_index+1] = '\0';
                argv[argc++] = &arguments[argument_index];
                argument_started = 1;
                
                // Clean up the argument variable because arguments
                // hold their values through commands and im lazy
            }
            
            arguments[argument_index++] = current_char;
        }

        // We now have the command name available, how many arguments
        // it holds and what arguments it has.
        // Now, we check if the command is registered in the command shell table
        for (int i = 0; i < command_count; i++) {
            shell_command_t* command = &commands[i];

            if (str_equals(command->name, command_name)) {
                // The command names match. We grab the command from the struct and
                // dispatch it.
                command_handler_t handler = command->handler;

                // Finally, we call the command.
                res = handler(argc, argv);
                break;
            }
        }

        // Reset buffer before reprocessing user input
        inputBufferLength = 0;

        switch (res) {
            case SS_BAD_CMD:
                println("Unknown command '");
                print(inputBuffer);
                print("'.");
                break;
            case SS_INVALID_ARGS:
                println("Insufficient arguments. Expected 0, received 0.");
                break;
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

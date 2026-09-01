#include<stdint.h>

#ifndef SHELL_H
#define SHELL_H

typedef int (*command_handler_t)(int argc, char ** argv);

typedef struct {
    char * name;
    uint8_t argc;
    command_handler_t handler;
} shell_command_t;

void shell_register(shell_command_t * cmd);

void register_cmd(char * name, int argc, command_handler_t command);

void shell_init();

void process_keyboard_input(char c, uint8_t code, uint8_t keyboard_map_len);

#endif
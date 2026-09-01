#include<stdint.h>

#include "../../kernel/terminal.h"

static inline uint8_t inb(unsigned short port) {
    uint8_t value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

void keyboard_init() {}

static char keyboard_map[] = {
    0,0,'1','2','3','4','5','6','7','8','9','0','"','=','\b',
    0,'q','w','e','r','t','y','u','i','o','p',0,0,'\n',
    0,'a','s','d','f','g','h','j','k','l',0,0,0,
    0,0,'z','x','c','v','b','n','m',0,0,0,0,
    0,0,' '
};

void keyboard_handler() {
    uint8_t code = inb(0x60);

    // Releasing a key
    if (code & 0x80) return;

    // Array is out of bounds, so we say that no key was found.
    // if (sizeof(keyboard_map) / sizeof(keyboard_map[0]) > code) {
    //     process_key(0);
    //     return;
    // }

    process_key(keyboard_map[code]);
}
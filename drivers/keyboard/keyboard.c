#include "../../kernel/terminal.h"

static inline char inb(unsigned short port) {
    unsigned char value;
    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

void keyboard_init() {}

void keyboard_handler() {
    unsigned char code = inb(0x60);

    (void) code;

    println("KEY!");
}
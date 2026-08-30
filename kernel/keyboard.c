#include "keyboard.h"

static inline unsigned char inb(unsigned short port)
{
    unsigned char value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

static const char keyboard_map[128] = {
    0,    27,   '1', '2', '3', '4', '5', '6',
    '7',  '8',  '9', '0', '-', '=', '\b', '\t',
    'q',  'w',  'e', 'r', 't', 'y', 'u', 'i',
    'o',  'p',  '[', ']', '\n', 0,   'a', 's',
    'd',  'f',  'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b',  'n',  'm', ',', '.', '/', 0,   '*',
    0,    ' ',  0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,   0,   0,   0,   0,   0
};

void keyboard_init() {}

char keyboard_getchar() {
    while (1) {
        // Status register.
        unsigned char status = inb(0x64);

        // Bit 0 = output buffer contains data.
        if (status & 1) {
            unsigned char scancode = inb(0x60);

            // Ignore key-release scancodes.
            if (scancode & 0x80)
                continue;

            return keyboard_map[scancode];
        }
    }
}

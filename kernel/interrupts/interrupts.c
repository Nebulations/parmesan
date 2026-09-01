#include<stdint.h>

#define NULL ((void *)0)

#include "drivers/keyboard/keyboard.h"

extern void keyboard_interrupt(void);

typedef void (*interrupt_handler_t)(void);

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;
static idt_entry_t idt_table[64];
static interrupt_handler_t idt_virtual_table[64];

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void interrupts_init() {
    uint32_t handler = (uint32_t) keyboard_interrupt;

    idt_table[33].offset_low = handler & 0xFFFF;
    idt_table[33].offset_high = (handler >> 16) & 0xFFFF;
    idt_table[33].selector = 0x08;
    idt_table[33].zero = 0;
    idt_table[33].flags = 0x8E;

    idt_virtual_table[33] = keyboard_handler;

    idt_ptr_t idt_ptr;
    idt_ptr.limit = sizeof(idt_table) - 1;
    idt_ptr.base = (uint32_t) idt_table;

    __asm__ volatile (
        "lidt %0"
        :
        : "m"(idt_ptr)
    );
}

void interrupt_dispatch(uint8_t id) {
    interrupt_handler_t handler = idt_virtual_table[id];

    if (handler != NULL) {
        handler();
    }
}

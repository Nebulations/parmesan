#include<stdint.h>

#define NULL ((void *)0)

#include "drivers/keyboard/keyboard.h"
#include "drivers/timings/pit/pit.h"

extern void keyboard_interrupt(void);
extern void pit_interrupt(void);

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

void register_interrupt(void (*interrupt)(void), interrupt_handler_t handler, int table_index) {
    uint32_t address = (uint32_t) interrupt;

    idt_table[table_index].offset_low = address & 0xFFFF;
    idt_table[table_index].offset_high = (address >> 16) & 0xFFFF;
    idt_table[table_index].selector = 0x08;
    idt_table[table_index].zero = 0;
    idt_table[table_index].flags = 0x8E;

    idt_virtual_table[table_index] = handler;
}

void interrupts_init() {
    register_interrupt(pit_interrupt, pit_handler, 32);
    register_interrupt(keyboard_interrupt, keyboard_handler, 33);

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

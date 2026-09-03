#include <stdint.h>

#include "lib/std/stdio.h"

#include "kernel/io/irq.h"

#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND 0x43
#define PIT_FREQUENCY 1193182

volatile uint32_t ticks;

void pit_init(uint32_t frequency);
uint32_t pit_get_ticks();

void pit_init(uint32_t frequency) {
    uint16_t divisor = PIT_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL_0, divisor & 0xFF);
    outb(PIT_CHANNEL_0, (divisor >> 8) & 0xFF);
}

void pit_handler() {
    ticks++;
}

uint32_t pit_get_ticks() {
    return ticks;
}
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

typedef void (*interrupt_handler_t)(void);

void interrupts_init();

interrupt_handler_t get_interrupt(int id);

#endif
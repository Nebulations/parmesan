#include "drivers/timings/pit/pit.h"

#include "lib/std/stdio.h"

/**
 * Sleep a given amount of time, in milliseconds.
 * Cannot sleep for less then 10ms.
 */
void sleep(uint32_t time) {
    uint32_t start = pit_get_ticks();

    // dumb stupid hack to get this working
    __asm__ volatile ("sti");

    while (pit_get_ticks() - start < (time/50)) {
        __asm__ volatile ("hlt");
    }
}
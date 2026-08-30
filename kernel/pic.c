#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

static void io_wait()
{
    outb(0x80, 0);
}

void pic_init()
{
    // Start initialization sequence
    outb(0x20, 0x11);
    io_wait();

    outb(0xA0, 0x11);
    io_wait();

    // Remap IRQs
    // Master PIC: IRQ 0-7  -> interrupts 32-39
    outb(0x21, 0x20);
    io_wait();

    // Slave PIC: IRQ 8-15 -> interrupts 40-47
    outb(0xA1, 0x28);
    io_wait();

    // Tell Master PIC that Slave PIC is connected to IRQ 2
    outb(0x21, 0x04);
    io_wait();

    // Tell Slave PIC its cascade identity
    outb(0xA1, 0x02);
    io_wait();

    // 8086 mode
    outb(0x21, 0x01);
    io_wait();

    outb(0xA1, 0x01);
    io_wait();

    // Enable IRQ 1 (keyboard)
    outb(0x21, 0xFD);

    // Keep all slave IRQs disabled for now
    outb(0xA1, 0xFF);
}
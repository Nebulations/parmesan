void outb(unsigned char c, unsigned short v) {
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(c), "Nd"(v)
    );
}
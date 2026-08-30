CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m32 -ffreestanding -nostdlib -T linker.ld

KERNEL = parmesan.bin

all: $(KERNEL)

compiled/boot.o: boot/boot.s
	$(AS) --32 boot/boot.s -o compiled/boot.o
compiled/interrupts_asm.o: boot/interrupts.s
	$(AS) --32 boot/interrupts.s -o compiled/interrupts_asm.o

# Kernel stuff
compiled/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o compiled/kernel.o
compiled/terminal.o: kernel/terminal.c
	$(CC) $(CFLAGS) -c kernel/terminal.c -o compiled/terminal.o
compiled/shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -c kernel/shell.c -o compiled/shell.o
compiled/memory.o: kernel/memory.c
	$(CC) $(CFLAGS) -c kernel/memory.c -o compiled/memory.o
compiled/heap.o: kernel/heap.c
	$(CC) $(CFLAGS) -c kernel/heap.c -o compiled/heap.o

compiled/interrupts.o: kernel/interrupts.c
	$(CC) $(CFLAGS) -c kernel/interrupts.c -o compiled/interrupts.o
compiled/pic.o: kernel/pic.c
	$(CC) $(CFLAGS) -c kernel/pic.c -o compiled/pic.o


# Utils
compiled/string.o: kernel/util/string.c
	$(CC) $(CFLAGS) -c kernel/util/string.c -o compiled/string.o

# Drivers
compiled/keyboard.o: drivers/keyboard/keyboard.c
	$(CC) $(CFLAGS) -c drivers/keyboard/keyboard.c -o compiled/keyboard.o

$(KERNEL): compiled/boot.o compiled/interrupts.o compiled/kernel.o compiled/terminal.o compiled/keyboard.o compiled/shell.o compiled/string.o compiled/memory.o compiled/heap.o compiled/interrupts_asm.o compiled/pic.o linker.ld
	$(LD) $(LDFLAGS) -o compiled/$(KERNEL) compiled/boot.o compiled/interrupts.o compiled/kernel.o compiled/terminal.o compiled/keyboard.o compiled/shell.o compiled/string.o compiled/memory.o compiled/heap.o compiled/interrupts_asm.o compiled/pic.o -lgcc

run: $(KERNEL)
	qemu-system-i386 -kernel compiled/$(KERNEL) -display gtk,grab-on-hover=off

CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m32 -ffreestanding -nostdlib -T linker.ld

KERNEL = parmesan.bin

all: $(KERNEL)

compiled/boot.o: boot/boot.s
	$(AS) --32 boot/boot.s -o compiled/boot.o

# Kernel stuff
compiled/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o compiled/kernel.o
compiled/terminal.o: kernel/terminal.c
	$(CC) $(CFLAGS) -c kernel/terminal.c -o compiled/terminal.o
compiled/keyboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) -c kernel/keyboard.c -o compiled/keyboard.o
compiled/shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -c kernel/shell.c -o compiled/shell.o
compiled/memory.o: kernel/memory.c
	$(CC) $(CFLAGS) -c kernel/memory.c -o compiled/memory.o
compiled/heap.o: kernel/heap.c
	$(CC) $(CFLAGS) -c kernel/heap.c -o compiled/heap.o

# Utils
compiled/string.o: kernel/util/string.c
	$(CC) $(CFLAGS) -c kernel/util/string.c -o compiled/string.o

$(KERNEL): compiled/boot.o compiled/kernel.o compiled/terminal.o compiled/keyboard.o compiled/shell.o compiled/string.o compiled/memory.o compiled/heap.o linker.ld
	$(LD) $(LDFLAGS) -o compiled/$(KERNEL) compiled/boot.o compiled/kernel.o compiled/terminal.o compiled/keyboard.o compiled/shell.o compiled/string.o compiled/memory.o compiled/heap.o -lgcc

run: $(KERNEL)
	qemu-system-i386 -kernel compiled/$(KERNEL) -display gtk,grab-on-hover=off

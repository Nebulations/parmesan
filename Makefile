CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-gcc

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m32 -ffreestanding -nostdlib -T linker.ld

KERNEL = parmesan.bin

# Find all C source files recursively
C_SOURCES = $(shell find kernel drivers -name "*.c")

# Convert:
# kernel/foo.c
# into:
# compiled/kernel/foo.o
OBJECTS = $(C_SOURCES:.c=.o)

# Put compiled/ in front of every object path
OBJECTS := $(addprefix compiled/,$(OBJECTS))

# Assembly sources
ASM_SOURCES = boot/boot.s boot/interrupts.s

ASM_OBJECTS = $(ASM_SOURCES:.s=.o)
ASM_OBJECTS := $(addprefix compiled/,$(ASM_OBJECTS))

all: $(KERNEL)

$(KERNEL): $(OBJECTS) $(ASM_OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -o compiled/$(KERNEL) $(OBJECTS) $(ASM_OBJECTS) -lgcc


# Compile C files
compiled/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


# Compile assembly files
compiled/%.o: %.s
	mkdir -p $(dir $@)
	$(AS) --32 $< -o $@


run: $(KERNEL)
	qemu-system-i386 -kernel compiled/$(KERNEL) -display gtk,grab-on-hover=off


clean:
	rm -rf compiled
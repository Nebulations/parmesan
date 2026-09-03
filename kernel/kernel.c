#include <stdint.h>

#include "drivers/display/vga/terminal.h"
#include "shell/shell.h"
#include "memory/memory.h"

#include "drivers/keyboard/keyboard.h"
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"
#include "drivers/timings/pit/pit.h"

#include "lib/std/stdio.h"

extern char kernel_end;

void debug_memory_map(unsigned int multiboot_info_addr);
void load_user();

void kernel_main(unsigned int multiboot_info_addr) {
    terminal_init();
    print("Loading...\n");

    // debug_memory_map(multiboot_info_addr);
    heap_init();
    shell_init();

    pic_init();
    pit_init(100);

    interrupts_init();
    
    // Enable CPU interrupts
    __asm__ volatile ("sti");

    load_user();
}

void debug_memory_map(unsigned int multiboot_info_addr) {
    multiboot_info_t* info = (multiboot_info_t*) multiboot_info_addr;

    if (!(info->flags & (1 << 6))) {
        return;
    }

    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)info->mmap_addr;

    while ((uint32_t) mmap < info->mmap_addr + info->mmap_length) {
        if (mmap->type == 1) {
            multiboot_memory_map_t* entry = (multiboot_memory_map_t*)((uint32_t) mmap);
            print("Start: ");
            printf("%d", entry->addr);
            print(" - End: ");
            printf("%d", entry->addr + entry->len);
            print(" - Type: ");
            if (entry->type == 1) {
                print("AVAILABLE");
            } else {
                print("RESERVED");
            }
            print("\n");
        }
        
        mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    uint32_t kernel_end = (uint32_t) &kernel_end;
}

void load_user() {
    print("Welcome to ParmesanOS!\n> ");
    draw_cursor();

    while (1) {
        __asm__ volatile ("hlt");
    }
}
#include <stdint.h>

#include "terminal.h"
#include "shell.h"
#include "memory.h"

#include "../drivers/keyboard/keyboard.h"
#include "interrupts.h"
#include "pic.h"

extern char kernel_end;

void debug_memory_map(unsigned int multiboot_info_addr);
void load_user();

void kernel_main(unsigned int multiboot_info_addr) {
    // Initialize the terminal to the end user.
    // Disable the hardware cursor in favor of the software cursor.
    disable_cursor();
    clear_screen();

    // debug_memory_map(multiboot_info_addr);
    heap_init();

    interrupts_init();
    pic_init();
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
            print_hex(entry->addr);
            print(" - End: ");
            print_hex(entry->addr + entry->len);
            print(" - Type: ");
            if (entry->type == 1) {
                print("AVAILABLE");
            } else {
                print("RESERVED");
            }
            println("");
        }
        
        mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    uint32_t kernel_end = (uint32_t) &kernel_end;
}

void load_user() {
    println("ParmesanOS");
    println("");
    println("> ");
    draw_cursor();

    while (1) {
        __asm__ volatile ("hlt");
    }
}
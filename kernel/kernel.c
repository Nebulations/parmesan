#include <stdint.h>

#include "terminal.h"
#include "keyboard.h"
#include "shell.h"
#include "memory.h"

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

    // Allocate a 128 byte buffer for user input.
    char inputBuffer[128];
    int inputBufferLength = 0;

    while (1) {
        char c = keyboard_getchar();

        if (c == 0) {
            continue;
        }

        // Check if the char is 'enter'
        if (c == '\n') {
            undraw_cursor();
            set_cursor(0, get_cursor_y()+1);

            inputBuffer[inputBufferLength] = '\0';

            int res = shell_process(inputBuffer);
            // Reset buffer before reprocessing user input
            inputBufferLength = 0;

            if (res != 0) {
                if (res == 1) {
                    return;
                }
                println("Error!");
            }

            println("> ");
            draw_cursor();

            continue;
        }

        // Check if we're backspacing/deleting chars
        if (c == '\b') {
            // We already deleted everything so we do nothing.
            if (inputBufferLength == 0) {
                continue;
            }

            undraw_cursor();
            set_cursor(get_cursor_x()-1, get_cursor_y());
            inputBufferLength--;
            draw_cursor();
            continue;
        }

        // Print the character to the terminal.
        print_char(c);
        draw_cursor();

        inputBuffer[inputBufferLength] = c;
        inputBufferLength++;
    }
}
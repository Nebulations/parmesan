#include<stdint.h>

#define NULL ((void *)0)

// #define HEAP_START 0x00107000
// #define HEAP_END 0x07FE0000

#define HEAP_START 0x00107000
#define HEAP_END 0x07FE0000

// Multiboot structs
typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
} multiboot_info_t;

struct multiboot_mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed));

typedef struct multiboot_mmap_entry multiboot_memory_map_t;

// Memory blocks
typedef struct {
    uint32_t size; // How many bytes the block can hold
    uint8_t allocated;  // 0 if its free, 1 if already allocated
} block_t;

static block_t* first_block;

void heap_init() {
    first_block = (block_t*) HEAP_START;
    first_block->size = HEAP_END - HEAP_START - sizeof(block_t);
    first_block->allocated = 0;
}

// Memory management
void* malloc(uint32_t bytes);
void free(void * addr);

void* malloc(uint32_t bytes) {
    // No bytes required to malloc, so we return a null pointer
    if (bytes == 0) {
        return NULL;
    }

    // First block is not allocated -> We don't use it.
    block_t* current_block = NULL;
    
    // Go through all possible memory blocks.
    while (1) {
        if (current_block == NULL) {
            // Current block is not initialized, so we use the default first block.
            current_block = first_block;
        } else {
            // Current block is already initialized. We move the pointer to the next header block,
            // located size of header + size of block away.
            // We also add the current address so we don't skip forward too much.
            current_block = (block_t*)((uint8_t*)current_block + sizeof(block_t) + current_block->size);
        }

        // Could not find a memory block in the heap -> return NULL.
        if ((uint8_t*)current_block >= HEAP_END) {
            return NULL;
        }

        // Current block is already allocated -> We skip it
        if (current_block->allocated == 1) {
            continue;
        }

        // Current block's size is too small for what we're looking for -> We skip it
        if (current_block->size < bytes) {
            continue;
        }

        // The memory block is free and has enough bytes to hold our data.

        // Block has the exact amount of bytes we're looking for
        if (current_block->size == bytes) {
            // Mark the block as allocated
            current_block->allocated = 1;

            // Return a void pointer pointing to the address of the start of the memory block.
            // Since the available memory is after the header, we add the header aswell to
            // obtain the memory address at the start of the data section.
            return (void*)((uint8_t*)current_block + sizeof(block_t));
        }

        // Block is too big -> We split it.
        //                              add +4 here so we don't have a memory block with 0 bytes of data.
        if (current_block->size >= (bytes + sizeof(block_t) + 4)) {

            // current_block now becomes the memory area we need to output.
            
            uint32_t old_size = current_block->size;

            // We define the current block as what we need for the new one
            current_block->size = bytes;
            current_block->allocated = 1;

            // Create a new header block for the unallocated remains of the memory block.
            block_t* new_block = (block_t*) ((uint8_t*)current_block + sizeof(block_t) + bytes);

            // Mark the newly created block as unallocated with the correct size.
            new_block->size = old_size - (sizeof(block_t) + bytes);
            new_block->allocated = 0;

            // Finally, we return the newly allocated memory block.
            return (void*)((uint8_t*)current_block + sizeof(block_t));
        }
    }

    return NULL;
}

void free(void * addr) {
    // The header is located on the left of the initial data address
    block_t* header = (block_t*) (addr - sizeof(block_t));

    // Mark the header to be free
    header->allocated = 0;

    // Then, we find the next available header.
    block_t* next_header = header;
    // Continue looping until we meet a header already allocated
    while (next_header->allocated == 0) {
        // The next header is the next one when we skip the header and bytes of said header.
        // This lands us directly on the next header, where we can convert it into a struct.
        next_header = (block_t*) (((uint8_t*) next_header) + sizeof(block_t) + next_header->size);

        // The next header is allocated -> We stop.
        if (next_header->allocated == 1) {
            return;
        }

        // The heap has ended -> We stop.
        if ((uint8_t*)next_header >= HEAP_END) {
            return;
        }

        // The next header is free to use. We merge the next header and the original header.
        header->size += next_header->size + sizeof(block_t);
        // next_header disappears since it has been merged with the original header.
    }
}
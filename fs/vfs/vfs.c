#include<stdint.h>

typedef struct {
    char name[32];
    char path[128];
    uint32_t size;
} __attribute__((packed)) fs_header_t;

typedef struct {
    char name[32];
    char path[128];
    uint32_t size;
    char * file_start;
} vfs_node_t;

static char path[256] = "/";

void vfs_init() {

}

void vfs_setpath(char * dir) {
    unsigned int counter = 0;
    
    while (*dir != '\0') {
        path[counter] = *dir;
        dir++;
        counter++;
    }

    path[counter] = '\0';
}

char * vfs_path() {
    return path;
}
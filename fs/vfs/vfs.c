#include<stdint.h>

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
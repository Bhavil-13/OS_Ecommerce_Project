#ifndef SHM_H
#define SHM_H
#include<stdbool.h>
#define BLOCK_SIZE 4096
#define FILENAME "read_write_shm.c"

char * attach_memory_block(char *filename, int size);
bool detach_memory_block(char *block);
bool destroy_memory_block(char *filename);

#endif
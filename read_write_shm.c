#include"shm.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int write_to_shm(char *filename, int size, char *message){
    char *block = attach_memory_block(filename, size);
    if(block == NULL){
        printf("ERROR couldn't get block\n");
        return -1;
    }
    strncpy(block, message, BLOCK_SIZE);
    detach_memory_block(block);
}

int read_from_shm(char *filename, int size){
    char *block = attach_memory_block(filename, size);
    if(block == NULL){
        printf("Couldn't get the block");
        return -1;
    }
    printf("Reading: \"%s\"\n", block);
    detach_memory_block(block);
}
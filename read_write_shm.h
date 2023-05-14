#ifndef RW_SHM
#define RW_SHM

int write_to_shm(char *filename, int size, char *message);
int read_from_shm(char *filename, int size);

#endif
#ifndef LOCKS
#define LOCKS

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define PORT_NO 5000
#define ADMIN 1
#define USER 2

struct pds{
    int key, offset;
};

void read_lock(int fd, struct flock lock);
void write_lock(int fd, struct flock lock);
void product_write_lock(int fd, struct flock lock);
void cart_offset_read_lock(int fd, struct flock lock, int offset);
void cart_offset_write_lock(int fd, struct flock lock, int offset);
void unlock(int fd, struct flock lock);
int user_offset(int fd, int user_ID);
void file_open(int record_fd, int user_fd, int admin_fd, int cart_fd);

#endif
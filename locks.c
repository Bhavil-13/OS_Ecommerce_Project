#include"locks.h"
#include"product.h"

void read_lock(int fd, struct flock lock){
    lock.l_len = 0;
    lock.l_type = F_RDLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    fcntl(fd, F_SETLKW, &lock);
}

void write_lock(int fd, struct flock lock){
    lock.l_len = 0;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    fcntl(fd, F_SETLKW, &lock);
}

void product_write_lock(int fd, struct flock lock){
    lseek(fd, (-1)*sizeof(struct product), SEEK_CUR);
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_CUR;
    lock.l_start = 0;
    lock.l_len = sizeof(struct product);
    fcntl(fd, F_SETLKW, &lock);
}

void cart_offset_read_lock(int fd, struct flock lock, int offset){
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct cart);
    lock.l_start = offset;
    lock.l_type = F_RDLCK;
    fcntl(fd, F_SETLKW, &lock);
    lseek(fd, offset, SEEK_SET);
}

void cart_offset_write_lock(int fd, struct flock lock, int offset){
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct cart);
    lock.l_start = offset;
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    lseek(fd, offset, SEEK_SET);
}

void unlock(int fd, struct flock lock){
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
}

int user_offset(int fd, int user_ID){
    if (user_ID < 0){
        perror("Invalid user ID\n");
        return -1;
    }
    struct flock lock;
    setLockCust(fd, lock);
    struct pds P;

    while (read(fd, &P, sizeof(struct pds))){
        if (P.key == user_ID){
            unlock(fd, lock);
            return P.offset;
        }
    }
    unlock(fd, lock);
    return -1;
}

void file_open(int record_fd, int user_fd, int admin_fd, int cart_fd){
    int record_fd = open("records.txt", O_RDWR | O_CREAT, 0777);
    int cart_fd = open("carts.txt", O_RDWR | O_CREAT, 0777);
    int user_fd = open("users.txt", O_RDWR | O_CREAT, 0777);
    int admin_fd = open("admin.txt", O_RDWR | O_CREAT, 0777);
    lseek(admin_fd, 0, SEEK_END);
}
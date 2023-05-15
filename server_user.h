#ifndef SERV_USER
#define SERV_USER
#include"locks.h"
#include"product.h"

void post_products(int sock_fd, int record_fd);
void post_cart(int sock_fd, int cart_fd, int user_fd);
void add_product_to_cart(int sock_fd, int cart_fd, int record_fd, int user_fd);
void edit_cart(int sock_fd, int cart_fd, int record_fd, int user_fd);
void post_pe(int sock_fd, int cart_fd, int record_fd, int user_fd);
void add_user(int sock_fd, int cart_fd, int user_fd);

#endif
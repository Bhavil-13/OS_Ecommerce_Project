#ifndef CLIENT_USER
#define CLIENT_USER
#include"product.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int get_user_ID();
void get_cart(int user_ID, int sock_fd); // prints the cart
void add_item_to_cart(int user_ID, int sock_fd, struct product prod_item);
void update_cart(int sock_fd, int user_ID);
void pay_for_cart(int sock_fd, int user_ID);
float get_total(struct cart user_cart);
void generate_reciept(int sock_fd, struct cart user_cart, float total);
void register_customer(int sock_fd);

#endif
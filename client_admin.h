#ifndef CLIENT_ADMIN
#define CLIENT_ADMIN
#include"product.h"

void add_product(int sock_fd);
void delete_product(int sock_fd, struct product prod);
void update_price(int sock_fd);
void update_quantity(int sock_fd);

#endif
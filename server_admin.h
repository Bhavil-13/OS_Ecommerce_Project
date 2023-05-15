#ifndef SERV_ADMIN
#define SERV_ADMIN
#include"product.h"
#include"locks.h"

void add_products(int sock_fd, int admin_fd, int records_fd);
void delete_products(int sock_fd, int admin_fd, int records_fd, int P_ID);
void generate_receipt(int admin_fd, int records_fd);
void update_products_qty(int sock_fd, int admin_fd, int records_fd);
void update_products_cost(int sock_fd, int admin_fd, int records_fd);

#endif
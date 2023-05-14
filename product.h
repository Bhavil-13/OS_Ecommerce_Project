#ifndef PROD_STRUCT
#define PROD_STRUCT

struct product{
    int P_ID, qty;
    char *P_Name;
    float cost;
};

void print_prod(struct product node);
struct product take_input();
void get_all_products(int sock_fd);

#endif
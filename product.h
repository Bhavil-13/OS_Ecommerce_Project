#ifndef PROD_STRUCT
#define PROD_STRUCT

#define MAX_PRODS_IN_CART 10

struct product{
    int P_ID, qty;
    char *P_Name;
    float cost;
};

struct cart{
    int user_ID;
    struct product prod_list[MAX_PRODS_IN_CART];
};

void print_prod(struct product node);
struct product take_input();
void get_all_products(int sock_fd);

#endif
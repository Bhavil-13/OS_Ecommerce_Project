#include"server_admin.h"

void add_products(int sock_fd, int admin_fd, int records_fd){
    struct product prod, prod_rec;
    read(sock_fd, &prod, sizeof(struct product));
    char P_Name[20], resp[128];
    int P_ID, qty, check = 0;
    float cost;
    strcpy(P_Name, prod.P_Name);
    P_ID = prod.P_ID;
    cost = prod.cost;
    qty = prod.qty;
    struct flock lock;
    read_lock(records_fd, lock);
    while(read(records_fd, &prod_rec, sizeof(struct product))){
        if (prod_rec.P_ID == P_ID && prod_rec.qty > 0){
            write(sock_fd, "FAIL", sizeof("FAIL"));
            sprintf(resp, "Adding product id %d failed[duplicate]\n", P_ID);
            write(admin_fd, resp, strlen(resp));
            unlock(records_fd, lock);
            check = 1;
            return;
        }
    }
    if (!check){
        lseek(records_fd, 0, SEEK_END);
        prod_rec.P_ID = P_ID;
        strcpy(prod_rec.P_Name, P_Name);
        prod_rec.cost = cost;
        prod_rec.qty = qty;
        write(records_fd, &prod_rec, sizeof(struct product));
        write(sock_fd, "OKAY", sizeof("OKAY"));
        sprintf(resp, "P_ID: %d added successfully\n", P_ID);
        write(admin_fd, resp, strlen(resp));
        unlock(records_fd, lock);   
    }
}

void delete_products(int sock_fd, int admin_fd, int records_fd, int P_ID){
    struct flock lock;
    read_lock(records_fd, lock);
    char res[128];
    struct product prod;
    int check = 0;
    while (read(records_fd, &prod, sizeof(struct product))){
        if (prod.P_ID == P_ID){
            unlock(records_fd, lock);
            product_write_lock(records_fd, lock);
            prod.P_ID = -1;
            strcpy(prod.P_Name, "");
            prod.cost = -1;
            prod.qty = -1;
            write(records_fd, &prod, sizeof(struct product));
            write(sock_fd, "OKAY", sizeof("OKAY"));
            sprintf(res, "P_ID: %d Deleted Succesfully\n", P_ID);
            write(admin_fd, res, strlen(res));
            unlock(records_fd, lock);
            check = 1;
            return;
        }
    }
    if (!check){
        sprintf(res, "P_ID: %d Delete Failed[does not exist]", P_ID);
        write(admin_fd, res, strlen(res));
        write(sock_fd, "FAIL", sizeof("FAIL"));
        unlock(records_fd, lock);
    }
}

void generate_receipt(int admin_fd, int records_fd){
    struct flock lock;
    read_lock(records_fd, lock);
    write(admin_fd, "Current Inventory:\n", strlen("Current Inventory:\n"));
    write(admin_fd, "|     P_ID     |     P_Name     |     QTY     |     cost     |\n", strlen("|     P_ID     |     P_Name     |     QTY     |     cost     |\n"));
    lseek(records_fd, 0, SEEK_SET);
    struct product prod;
    while (read(records_fd, &prod, sizeof(struct product))){
        if (prod.P_ID != -1){
            char admin_storing[100];
            sprintf(admin_storing, "|     %d     |     %s     |     %d     |     %f     |\n",prod.P_ID, prod.P_Name, prod.qty, prod.cost);
            write(admin_fd, admin_storing, strlen(admin_storing));
        }
    }
    unlock(records_fd, lock);
}

void update_products_qty(int sock_fd, int admin_fd, int records_fd){
    struct product prod, prod_rec;
    read(sock_fd, &prod, sizeof(struct product));
    int P_ID = prod.P_ID;
    char res[128];
    int new_qty = prod.qty;
    struct flock lock;
    read_lock(records_fd, lock);
    int check = 0;
    while(read(records_fd, &prod_rec, sizeof(struct product))){
         if (prod.P_ID == P_ID){
            unlock(records_fd, lock);
            product_write_lock(records_fd, lock);
            int old_qty;
            old_qty = prod_rec.qty;
            prod_rec.qty = new_qty;
            write(records_fd, &prod_rec, sizeof(struct product));
            sprintf(res, "P_ID: %d quantity modified from: %d to: %d \n", P_ID, old_qty, new_qty);
            write(admin_fd, res, strlen(res));
            write(sock_fd, "OKAY", sizeof("OKAY"));               
            unlock(records_fd, lock);
            check = 1;
            break;
        }
    }
    if(!check){
        write(sock_fd, "FAIL", sizeof("FAIL"));
        unlock(records_fd, lock);
    }
}

void update_products_cost(int sock_fd, int admin_fd, int records_fd){
    struct product prod, prod_rec;
    read(sock_fd, &prod, sizeof(struct product));
    int P_ID = prod.P_ID;
    char res[128];
    float new_cost = prod.cost;
    struct flock lock;
    read_lock(records_fd, lock);
    int check = 0;
    while(read(records_fd, &prod_rec, sizeof(struct product))){
         if (prod.P_ID == P_ID){
            unlock(records_fd, lock);
            product_write_lock(records_fd, lock);
            float old_cost;
            old_cost = prod_rec.cost;
            prod_rec.cost = new_cost;
            write(records_fd, &prod_rec, sizeof(struct product));
            sprintf(res, "P_ID: %d cost modified from: %f to: %f \n", P_ID, old_cost, new_cost);
            write(admin_fd, res, strlen(res));
            write(sock_fd, "OKAY", sizeof("OKAY"));               
            unlock(records_fd, lock);
            check = 1;
            break;
        }
    }
    if(!check){
        write(sock_fd, "FAIL", sizeof("FAIL"));
        unlock(records_fd, lock);
    }
}

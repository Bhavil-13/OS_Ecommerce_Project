#include"server_user.h"

void post_cart(int sock_fd, int cart_fd, int user_fd){
    int user_ID, offset;
    if(read(sock_fd, &user_ID, sizeof(int)) < 0){
        perror("ERROR: Couldn't read Read");
        // exit(1);
    }
    offset = user_offset(cart_fd, user_ID);
    struct cart cart;
    if(offset == -1){
        cart.user_ID = -1;
        write(sock_fd, &cart, sizeof(struct cart));
        return;
    }
    struct flock lock;
    cart_offset_read_lock(cart_fd, lock, offset);
    read(cart_fd, &cart, sizeof(struct cart));
    write(sock_fd, &cart, sizeof(struct cart));
    unlock(cart_fd, lock);
}

void add_user(int sock_fd, int cart_fd, int user_fd){
    char ch;
    read(sock_fd, &ch, sizeof(char));
    if(ch != 'r'){
        perror("ERROR: Couldn't add user");
        return;
    }
    struct flock lock;
    read_lock(user_fd, lock);
    int send_id = -1;
    struct pds P;
    while(read(user_fd, &P, sizeof(struct pds))){
        if(P.key > send_id){
            send_id = P.key;
        }
    }
    send_id++;
    P.key = send_id;
    P.offset = lseek(cart_fd, 0, SEEK_END);
    lseek(user_fd, 0, SEEK_END);
    write(user_fd, &P, sizeof(struct pds));
    struct cart cart;
    cart.user_ID = send_id;
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        cart.prod_list[i].cost = -1;
        cart.prod_list[i].P_ID = -1;
        cart.prod_list[i].qty = -1;
        strcpy(cart.prod_list[i].P_Name, "");
    }
    write(cart_fd, &cart, sizeof(struct cart));
    unlock(user_fd, lock);    
    write(sock_fd, &send_id, sizeof(int));
}

void post_pe(int sock_fd, int cart_fd, int record_fd, int user_fd){
    //check the locks in this
    int user_ID;
    read(sock_fd, &user_ID, sizeof(int));
    int offset;
    offset = user_offset(user_fd, user_ID);
    if(offset == -1){
        perror("ERROR: Offset");
        return;
    }
    write(sock_fd, &offset, sizeof(int));
    struct flock lock;
    cart_offset_read_lock(cart_fd, lock, offset);
    struct cart cart;
    read(cart_fd, &cart, sizeof(struct cart));
    unlock(cart_fd, lock);
    write(sock_fd, &cart, sizeof(struct cart));
    float total = 0.0;
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        if(cart.prod_list[i].P_ID != -1){
            write(sock_fd, &cart.prod_list[i].qty, sizeof(int));
            struct flock lock;
            read_lock(record_fd, lock);
            lseek(record_fd, 0, SEEK_SET);
            struct product prod;
            int check = 0;
            while(read(record_fd, &prod, sizeof(struct product))){
                if(prod.P_ID == cart.prod_list[i].P_ID && prod.qty > 0){
                    int min_order;
                    if (cart.prod_list[i].qty >= prod.qty){
                        min_order = prod.qty;
                    }else{
                        min_order = cart.prod_list[i].qty;
                    }
                    check =1;
                    write(sock_fd, &min_order, sizeof(int));
                    write(sock_fd, &prod.cost, sizeof(int));
                    break;
                }
            }
            unlock(record_fd, lock);
            if(!check){
                int send = 0;
                write(sock_fd, &send, sizeof(int));
                write(sock_fd, &send, sizeof(int));
            }
        }
    }
    char ch;
    read(sock_fd,  &ch, sizeof(char));
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        struct flock lock;
        read_lock(record_fd, lock);
        lseek(record_fd, 0, SEEK_SET);
        struct product prod;
        while(read(record_fd, &prod, sizeof(struct product))){
            if(prod.P_ID == cart.prod_list[i].P_ID){
                int min;
                if(cart.prod_list[i].qty >= prod.qty){
                    min = prod.qty;
                } else{
                    min = cart.prod_list[i].qty;
                }
                unlock(record_fd, lock);
                product_write_lock(record_fd, lock);
                prod.qty = prod.qty - min;
                write(record_fd, &prod, sizeof(struct product));
                unlock(record_fd, lock);
            }
        }
        unlock(record_fd, lock);
    }
    cart_offset_write_lock(cart_fd, lock, offset);
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        cart.prod_list[i].cost = -1;
        cart.prod_list[i].P_ID = -1;
        cart.prod_list[i].qty = -1;
        strcpy(cart.prod_list[i].P_Name, "");
    }
    write(cart_fd, &cart, sizeof(struct cart));
    write(sock_fd, &ch, sizeof(char));
    unlock(cart_fd, lock);
    read(sock_fd, &total, sizeof(float));
    read(sock_fd, &cart, sizeof(struct cart));
    int record = open("records.txt", O_CREAT | O_RDWR, 0777);
    write(record, "|     P_ID     |     NAME     |     QNTY     |     COST     |", sizeof("|     P_ID     |     NAME     |     QNTY     |     COST     |"));
    char temp[100];
    for (int i = 0; i < MAX_PRODS_IN_CART; i++){
        if (cart.prod_list[i].P_ID != -1){
            sprintf(temp, "|     %d     |     %s     |     %d     |     %f     |\n", cart.prod_list[i].P_ID, cart.prod_list[i].P_Name, cart.prod_list[i].qty, cart.prod_list[i].cost);
            write(record, temp, strlen(temp));
        }
    }
    sprintf(temp, "Total - %f\n", total);
    write(record, temp, strlen(temp));
    close(record);
}

void edit_cart(int sock_fd, int cart_fd, int record_fd, int user_fd){
    int user_ID = -1;
    read(sock_fd, &user_ID, sizeof(int));
    int offset = user_offset(user_fd, user_ID);
    write(sock_fd, &offset, sizeof(int));
    if(offset == -1){
        perror("ERROR: user_offset");
        return;
    }
    struct flock lock;
    cart_offset_read_lock(cart_fd, lock, offset);
    struct cart cart;
    read(cart_fd, &cart, sizeof(struct cart));
    struct product prod;
    read(sock_fd, &prod, sizeof(struct product));
    int P_ID = prod.P_ID;
    int qty = prod.qty;
    int check = 0, index = 0;
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        if(cart.prod_list[i].P_ID == P_ID){
            struct flock lock_product;
            read_lock(record_fd, lock_product);
            struct product prod_new;
            while(read(record_fd, &prod_new, sizeof(struct product))){
                if(prod_new.P_ID == P_ID && prod_new.qty > 0){
                    if(prod_new.qty > qty){
                        check = 1;
                        break;
                    } else {
                        check = 0;
                        break;
                    }
                }
            }
            index = i;
            unlock(record_fd, lock_product);
            break;
        }
    }
    unlock(cart_fd, lock);
    if(!check){
        write(sock_fd, "FAIL", sizeof("FAIL"));
        return;
    }
    cart.prod_list[index].qty = qty;
    write(sock_fd, "OKAY", sizeof("OKAY"));
    cart_offset_write_lock(cart_fd, lock, offset);
    write(cart_fd, &cart, sizeof(struct cart));
    unlock(cart_fd, lock);
}

void add_product_to_cart(int sock_fd, int cart_fd, int record_fd, int user_fd){
    int user_ID = -1, check = 0;
    read(sock_fd, &user_ID, sizeof(int));
    int offset = user_offset(user_fd, user_ID);
    write(sock_fd, &offset, sizeof(int));
    if(offset == -1){
        perror("ERROR: user_offset");
        return;
    }
    struct flock lock;
    cart_offset_read_lock(cart_fd, lock, offset);
    struct cart cart;
    read(cart_fd, &cart, sizeof(struct cart));
    struct flock lock_product;
    read_lock(record_fd, lock_product);
    struct product prod, prod_rec;
    read(sock_fd, &prod, sizeof(struct product));
    int P_ID = prod.P_ID;
    int qty = prod.qty;
    while(read(record_fd, &prod_rec, sizeof(struct product))){
        if(prod_rec.P_ID == prod.P_ID){
            if(prod_rec.qty >= prod.qty){
                check = 1;
                break;
            }
        }
    }
    unlock(cart_fd, lock);
    unlock(record_fd, lock_product);
    if(!check){
        write(sock_fd, "FAIL", sizeof("FAIL"));
    }
    check = 0;
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        if(cart.prod_list[i].P_ID == prod.P_ID){
            check = 1;
            break;
        }
    }
    if(check){
        write(sock_fd, "FAIL", sizeof("FAIL"));
        return;
    }
    check = 0;
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        if (cart.prod_list[i].P_ID == -1){
            check = 1;
            cart.prod_list[i].P_ID = prod.P_ID;
            cart.prod_list[i].qty = prod.qty;
            strcpy(cart.prod_list[i].P_Name, prod_rec.P_Name);
            cart.prod_list[i].cost = prod_rec.cost;
            break;
        }else if (cart.prod_list[i].qty <= 0){
            check = 1;
            cart.prod_list[i].P_ID = prod.P_ID;
            cart.prod_list[i].qty = prod.qty;
            strcpy(cart.prod_list[i].P_Name, prod_rec.P_Name);
            cart.prod_list[i].cost = prod_rec.cost;
            break;
        }
    }
    if(!check){
        write(sock_fd, "FAIL", sizeof("FAIL"));
        return;
    }
    write(sock_fd, "OKAY", sizeof("OKAY"));
    cart_offset_write_lock(cart_fd, lock, offset);
    write(cart_fd, &cart, sizeof(struct cart));
    unlock(cart_fd, lock);
}
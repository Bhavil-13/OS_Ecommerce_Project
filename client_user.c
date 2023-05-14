#include"client_user.h"
#include<stdio.h>
#include<string.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int get_user_ID(){
    printf("Enter the user ID: ");
    int user_id;
    scanf("%d", user_id);
    return user_id;
}

void get_cart(int user_ID, int sock_fd){
    write(sock_fd, &user_ID, sizeof(int));
    sleep(1);
    struct cart user_cart;
    read(sock_fd, &user_cart, sizeof(struct cart));
    if(user_cart.user_ID < 0){
        error("Invalid ID");
    }
    else{
        printf("Customer ID: %d", user_cart.user_ID);
        for(int i = 0; i < MAX_PRODS_IN_CART; i++){
            print_prod(user_cart.prod_list[i]);
        }        
    }
}

void add_item_to_cart(int user_ID, int sock_fd, struct product prod){
    write(sock_fd, &user_ID, sizeof(int));
    sleep(1);
    int res;
    read(sock_fd, &res, sizeof(int));
    if(res < 0){
        error("Invalid user_ID");
    }
    write(sock_fd, &prod, sizeof(struct product));
    char response[4];// either OKAY or FAIL
    read(sock_fd, response, sizeof(response));
    if(strncmp(response, "OKAY", 4)){
        printf("Successfully Added product with P_ID: %d to the cart", prod.P_ID);
    }else{
        error("Couldn't add to the cart!");
    }
}

void update_cart(int user_ID, int sock_fd, struct product old_prod, struct product new_prod){
    write(sock_fd, &user_ID, sizeof(int));
    sleep(1);
    int res;
    read(sock_fd, &res, sizeof(int));
    if(res < 0){
        error("Invalid user_ID");
    }
    /*
    Do the Update here
    */
}

void pay_for_cart(int sock_fd, int user_ID){
    write(sock_fd, &user_ID, sizeof(int));
    int res;
    read(sock_fd, &res, sizeof(int));
    if(res < 0){
        error("Invalid user_ID");
    }
    struct cart user_cart;
    read(sock_fd, &user_cart, sizeof(user_cart));
    int ordered, final_ordered, cost;
    //final_ordered, in case ordered is more that the total amount of product,
    //then we will get the total amount instead of an error.
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        if(user_cart.prod_list[i].P_ID >= 0){
            read(sock_fd, &ordered, sizeof(int));
            read(sock_fd, &final_ordered, sizeof(int));
            read(sock_fd, &cost, sizeof(int));
            printf("|     ITEM     |     QTY     |     PRICE     |");
            printf("     %s:     |     %d     |     %f", user_cart.prod_list[i].P_Name, final_ordered, cost * final_ordered);
            user_cart.prod_list[i].cost = cost;
            user_cart.prod_list[i].qty = final_ordered;
        }
    }
    float checkout_price = get_total(user_cart);
    printf("Total Price                |     %f\n", checkout_price);
    printf("After Tax Price            |     %f\n", checkout_price*1.15);
    printf("PAY: ");
    float payment;
    scanf("%f", payment);
    if(payment != checkout_price * 1.15){
        error("Insufficient Balance");
    }
    printf("Payment Successful");
    char ch = 'y';
    write(sock_fd, &ch, sizeof(char));
    read(sock_fd, &ch, sizeof(char));
    generate_reciept(sock_fd, user_cart, checkout_price * 1.15);
}

float get_total(struct cart user_cart){
    float total = 0.00;
    for(int i = 0; i < MAX_PRODS_IN_CART; i++){
        if(user_cart.prod_list[i].P_ID != -1){
            total += user_cart.prod_list[i].qty * user_cart.prod_list[i].cost;
        }
    }
    return total;
}

void generate_reciept(int sock_fd, struct cart user_cart, float total){
    write(sock_fd, &total, sizeof(float));
    write(sock_fd, &user_cart, sizeof(struct cart));
}

void register_customer(int sock_fd){
    char ch = 'y';
    write(sock_fd, &ch, sizeof(char));
    int id;
    printf("Registering new customer...\n");
    sleep(1);
    read(sock_fd, &id, sizeof(int));
    printf("Successfully registered new customer\n");
    printf("Your Customer ID is : %d", id);
}
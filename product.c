#include"product.h"
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

void print_prod(struct product prod){
    printf("P_ID: %d\nP_Name: %s\nCost: %f\nQuantity: %d", prod.P_ID, prod.P_Name, prod.cost, prod.qty);
}

struct product take_input(){
    struct product input_prod;
    printf("Enter the P_ID: ");
    scanf("%d", &input_prod.P_ID);
    printf("Enter product quantity: ");
    scanf("%d", &input_prod.qty);
    //the next 2 will be changed, they will be mapped with P_ID and we won't have to take them in input.
    printf("Enter product cost: ");
    scanf("%f", &input_prod.cost);
    printf("Enter product name: ");
    scanf("%s", input_prod.P_Name);
    return input_prod;
}



void get_all_products(int sock_fd){
    while(1){
        struct product prod;
        read(sock_fd, &prod, sizeof(struct product));
        if(prod.P_ID < 0){
            break;
        }
        else{
            print_prod(prod);
        }
    }
}
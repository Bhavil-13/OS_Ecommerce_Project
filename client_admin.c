#include"client_admin.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

void add_product(int sock_fd){
    struct product prod;
    // prod = take_input();
    printf("Enter the P_ID: ");
    scanf("%d", &prod.P_ID);
    printf("Enter product quantity: ");
    scanf("%d", &prod.qty);
    //the next 2 will be changed, they will be mapped with P_ID and we won't have to take them in input.
    printf("Enter product cost: ");
    scanf("%f", &prod.cost);
    printf("Enter product name: ");
    // scanf("%[^\n]%*c", prod.P_Name);
    fgets(prod.P_Name, 20, stdin);
    // scanf("%s", prod.P_Name);
    printf("Writing...");
    write(sock_fd, &prod, sizeof(struct product));
    printf("Done!");
    char res[4];
    read(sock_fd, res, sizeof(res));
    if(strcmp(res, "FAIL")){
        error("Couldn't add the product\n");
    }
}

void delete_product(int sock_fd, int P_ID){
    write(sock_fd, &P_ID, sizeof(int));
    char res[4];
    read(sock_fd, res, sizeof(res));
    if(strcmp(res, "FAIL")){
        error("Couldn't delete the product\n");
    }    
}

void update_price(int sock_fd){
    struct product prod = take_input();
    //this will have a prod's pid, pname and qty as before in input, but with a different price
    //gets the correspoing product stored and update it in server, here, only request is sent
    write(sock_fd, &prod, sizeof(struct product));
    char res[4];
    read(sock_fd, res, sizeof(res));
    if(strcmp(res, "FAIL")){
        error("Couldn't update the product's price\n");
    }
}

void update_quantity(int sock_fd){
    struct product prod = take_input();
    //this will have a prod's pid, pname and price as before in input, but with a different qty
    //gets the correspoing product stored and update it in server, here, only request is sent
    write(sock_fd, &prod, sizeof(struct product));
    char res[4];
    read(sock_fd, res, sizeof(res));
    if(strcmp(res, "FAIL")){
        error("Couldn't update the product's quantity\n");
    }
}
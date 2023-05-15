#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include"connect_socket.h"
#include"menues.h"
#include"client_user.h"
#include"client_admin.h"

#define ADMIN 1
#define USER 2
#define PORT_NO 5000

int main(){
    int sock_fd;
    client_connect(sock_fd, PORT_NO);
    int menue_choice = show_main_menue();
    if(menue_choice == USER){
        while(1){
            int user_ID;
            printf("Enter the user ID: ");
            scanf("%d", &user_ID);
            int choice = show_user_menue();
            write(sock_fd, &choice, sizeof(int));
            if(choice == 1){
                get_all_products(sock_fd);
            }else if(choice == 2){
                get_cart(user_ID, sock_fd);
            }else if(choice == 3){
                printf("Enter the details for the product you want to buy: \n");
                add_item_to_cart(user_ID, sock_fd, take_input());// see if it works properly
            }else if(choice == 4){
                update_cart(sock_fd, user_ID);
            }else if(choice == 5){
                pay_for_cart(sock_fd, user_ID);
            }else if(choice == 6){
                register_customer(sock_fd);
            }
            else if(choice == 7){
                //instead, make the go back thing using clear screen
                break;
            }
        }
    }
    else if(menue_choice == ADMIN){
        while(1){
            int choice = show_admin_menue();
            write(sock_fd, &choice, sizeof(int));
            if(choice == 1){
                add_product(sock_fd);
            }else if(choice == 2){
                int P_ID;
                printf("Enter the P_ID: ");
                scanf("%d", &P_ID);
                delete_product(sock_fd, P_ID);
            }else if(choice == 3){
                update_price(sock_fd);
            }else if(choice == 4){
                update_quantity(sock_fd);
            }else if(choice == 5){
                get_all_products(sock_fd);
            }else if(choice == 6){
                //instead, make the go back thing using clear screen
                break;
            }
        }
    }
    close_socket(sock_fd);
    return 0;
}
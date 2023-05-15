#include "server.h"
#include "connect_socket.h"

int main()
{
    int record_fd, user_fd, admin_fd, cart_fd;
    file_open(record_fd, user_fd, admin_fd, cart_fd);
    int sock_fd, accept_sock_fd;
    // accept() might give some problems, put it inside a while(1) if it does
    accept_sock_fd = server_connect(sock_fd, PORT_NO);
    if (!fork())
    {
        close(sock_fd);
        int menue_choice;
        read(accept_sock_fd, &menue_choice, sizeof(int));
        if (menue_choice == ADMIN)
        {
            while (1)
            {
                int choice;
                read(accept_sock_fd, &choice, sizeof(int));
                lseek(record_fd, 0, SEEK_SET);
                lseek(cart_fd, 0, SEEK_SET);
                lseek(user_fd, 0, SEEK_SET);
                if(menue_choice == 1){
                    printf("Adding product, choice was: %d", choice);
                    add_products(accept_sock_fd, admin_fd, record_fd);
            }else if(menue_choice == 2){
                int P_ID;
                read(accept_sock_fd, &P_ID, sizeof(int));
                delete_products(accept_sock_fd, admin_fd, record_fd, P_ID);
            }else if(menue_choice == 3){
                update_products_cost(accept_sock_fd, admin_fd, record_fd);
            }else if(menue_choice == 4){
                update_products_qty(accept_sock_fd, admin_fd, record_fd);
            }else if(menue_choice == 5){
                post_products(accept_sock_fd, record_fd);
            }else if(menue_choice == 6){
                close_socket(accept_sock_fd);
                generate_receipt(admin_fd, record_fd);
            }
            else{
                continue;
            }
            }
        }
        else if (menue_choice == USER){
            while (1){
                int choice;
                read(accept_sock_fd, &choice, sizeof(int));
                lseek(record_fd, 0, SEEK_SET);
                lseek(cart_fd, 0, SEEK_SET);
                lseek(user_fd, 0, SEEK_SET);
                if (choice == 1){
                    post_products(accept_sock_fd, record_fd);
                }
                else if (choice == 2){
                    post_cart(accept_sock_fd, cart_fd, user_fd);
                }
                else if (choice == 3){
                    add_product_to_cart(accept_sock_fd, cart_fd, record_fd, user_fd);
                }
                else if (choice == 4){
                    edit_cart(accept_sock_fd, cart_fd, record_fd, user_fd);
                }
                else if (choice == 5){
                    post_pe(accept_sock_fd, cart_fd, record_fd, user_fd);
                }
                else if (choice == 6){
                    add_user(accept_sock_fd, cart_fd, user_fd);
                }
                else if (choice == 7){
                    close_socket(accept_sock_fd);
                    break;
                }
                else{
                    continue;
                }
            }
        }
        else{
            close_socket(accept_sock_fd);
        }
    }
    return 0;
}
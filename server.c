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
                    // get_all_products(sock_fd);
                }
                else if (choice == 2){
                    // get_cart(user_ID, sock_fd);
                }
                else if (choice == 3){
                    // printf("Enter the details for the product you want to buy: \n");
                    // add_item_to_cart(user_ID, sock_fd, take_input());// see if it works properly
                }
                else if (choice == 4){
                    // update_cart(sock_fd, user_ID);
                }
                else if (choice == 5){
                    // pay_for_cart(sock_fd, user_ID);
                }
                else if (choice == 6){
                    // register_customer(sock_fd);
                }
                else if (choice == 7){
                    // instead, make the go back thing using clear screen
                    //  break;
                }
            }
        }
    }
    return 0;
}
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
    if(menue_choice == ADMIN){
        while(1){
            int choice = show_admin_menue();
            write(sock_fd, &choice, sizeof(int));
            if(choice == 1){

            }else if(choice == 2){

            }else if(choice == 3){

            }else if(choice == 4){

            }else if(choice == 5){

            }else if(choice == 6){
                break;
            }
        }
    }
    else if(menue_choice == USER){
        while(1){
            int choice = show_user_menue();
            write(sock_fd, &choice, sizeof(int));
            if(choice == 1){

            }else if(choice == 2){

            }else if(choice == 3){

            }else if(choice == 4){

            }else if(choice == 5){

            }else if(choice == 6){
                break;
            }
        }
    }
    close_socket(sock_fd);
    return 0;
}
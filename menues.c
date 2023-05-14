#include"menues.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int show_admin_menue(){
    printf("=====ADMIN MENUE=====\n1. Add Product\n2. Delete Product\n3. Update\n4. Logout\n5. Send\n6. Exit\nENTER YOUR CHOICE: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

int show_user_menue(){
    printf("=====USER MENUE=====\n1. Display Products\n2. Display Cart\n3. Choose Products\n4. Edit Cart\n5. Get\n6. Exit\n7. Logout\nENTER YOUR CHOICE: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

int show_main_menue(){
    printf("=====MAIN MENUE=====\n1. Login As Admin\n2. Login As User\n ENTER YOUR CHOICE: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}
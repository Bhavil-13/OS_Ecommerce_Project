#include"connect_socket.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int server_connect(int socket_fd, int port_no){
    //server_side connect
    // returns the new_sock_fd after accepting
    int newsockfd, n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    printf("Creating a Socket...\n");
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        error("Error opening the socket");
        return -1;
    }
    printf("Successfully created a socket with socket_fd: %d\n", socket_fd);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_no);
    printf("Binding to port number: %d\n", port_no);
    if(bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Binding failed");
        return -1;
    }
    printf("Successfully binded\nNow listening...\n");
    listen(socket_fd, 2);
    clilen = sizeof(cli_addr);
    newsockfd = accept(socket_fd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd < 0){
        error("Error in accept");
        return -1;
    }
    printf("Accepted the connection!");
    return newsockfd;
}

int client_connect(int socket_fd, int port_no){
    int socket_fd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    printf("Creating a Socket...\n");
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        error("Error opening the socket");
        return -1;
    }
    printf("Successfully created a socket with socket_fd: %d\n", socket_fd);
    server = gethostbyname("127.0.0.1");
    if(server == NULL){
        fprintf(stderr, "Error, no such host");
        return -1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_no);
    printf("Connecting...");
    if(connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Connection failed");
        return -1;
    }
    printf("Connected!");
    return 0;
}

void close_socket(int socket_fd){
    close(socket_fd);
}
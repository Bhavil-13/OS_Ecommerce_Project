#ifndef SOCKET_CON
#define SOCKET_CON

int server_connect(int socket_fd, int port_no);
int client_connect(int socket_fd, int port_no);
void close_socket(int socket_fd);

#endif
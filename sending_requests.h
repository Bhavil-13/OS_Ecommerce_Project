#ifndef REQUESTS
#define REQUESTS

int client_to_server(int socket_fd, void *request);
int server_to_client(int socket_fd, void *request);

#endif
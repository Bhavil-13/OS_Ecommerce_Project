for client:
gcc -o client client_admin.h client_admin.c client_user.h client_user.c client.c connect_socket.h connect_socket.c login.h menues.h menues.c product.h product.c read_write_shm.h read_write_shm.c sending_requests.h shm.h shm.c locks.h locks.c

for server:
gcc -o server connect_socket.h connect_socket.c locks.h locks.c product.h product.c server_admin.h server_admin.c server_user.h server_user.c server.h server.c
//socket.h
#include<arpa/inet.h> //inet_addr

#define PTHREAD_MAX 50
typedef struct {
	int socket;
	int pthreads_count;
	int client_sockets[PTHREAD_MAX];
	int pthreads_max;
	int do_close;
} server_socket;

typedef struct {
	int socket;	
	void *data;
	server_socket *server;
} socket_client_context;


int server_socket_open(server_socket *,unsigned int port);
void server_socket_close(server_socket *);
void server_socket_listen(server_socket *, int backlog);
int server_socket_waitclients_threaded(server_socket *, int *handler(socket_client_context *), void *data);
int server_socket_waitclients(server_socket *, int *handler(socket_client_context *), void *data);


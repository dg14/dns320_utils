//test_sockets.c
// main.c

#include <stdio.h>
#include "str.h"
#include <string.h>
#include <stdlib.h>
#include "socket.h"

int *handler(socket_client_context *ctx) {
    //Get the socket descriptor
    int sock = ctx->socket;
    int read_size;
    char *message , client_message[2000];

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , str_len(message));

    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , str_len(message));

    //Receive a message from client
    while ( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        write(sock , client_message , str_len(client_message));
    }

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(ctx);

    return 0;
}

int main(int argc, char *argv[]) {
    server_socket server;
    int server_socket;
    if ((server_socket_open(&server, 5000)) < 0) {
        return;
    }
    server_socket_listen(&server, 3);
    server_socket_waitclients_threaded(&server, &handler, NULL);

}


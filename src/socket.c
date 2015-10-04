//socket.c

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include "log.h"
#include "socket.h"

int server_socket_open(server_socket *server, unsigned int port) {
    struct sockaddr_in server_addr;
    server->do_close=0;
    //Create socket
    server->socket = socket(AF_INET , SOCK_STREAM , 0);
    server->pthreads_max = PTHREAD_MAX;
    if (server->socket == -1)
    {
        log_log(LOG_ERROR, "ServerSocket: Could not create socket");
        return -1;
    }
    log_log(LOG_DEBUG, "ServerSocket: Socket created");

    //Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons( port );

    int b=bind(server->socket, (struct sockaddr *)&server_addr , sizeof(server_addr));
    if (b<0) {
        log_log(LOG_ERROR, "ServerSocket: Could not bind");
        return -1;
    }
    log_log(LOG_DEBUG, "ServerSocket: Bind done");
    return 0;
}

void server_socket_listen(server_socket *server, int backlog) {
    listen(server->socket, backlog);
}

int server_socket_waitclients_threaded(server_socket *server, int *handler(socket_client_context *), void *data) {
    log_log(LOG_DEBUG, "ServerSocket: Waiting for incoming connections...");
    int c = sizeof(struct sockaddr_in);
    int client_socket;
    struct sockaddr_in client;
    socket_client_context *ctx;

    while ( (client_socket = accept(server->socket, (struct sockaddr *)&client, (socklen_t*)&c)) && server->do_close==0 )
    {
        if (server->pthreads_count == PTHREAD_MAX) {
            log_log(LOG_DEBUG, "ServerSocket: Connection not accepted, threads limit");                
            close(client_socket);
        } else {
            log_log(LOG_DEBUG, "ServerSocket: Connection accepted");
            pthread_t sniffer_thread;
            ctx = malloc(sizeof(socket_client_context));
            ctx->socket = client_socket;
            ctx->data = data;
            ctx->server=server;
            int pthread_number;
            if ( (pthread_number = pthread_create( &sniffer_thread , NULL ,  handler , (void*) ctx)) < 0)
            {
                log_log(LOG_ERROR, "ServerSocket: Could not create thread");
                return 1;
            }
            server->client_sockets[server->pthreads_count++] = client_socket;

            //Now join the thread , so that we dont terminate before the thread
            //pthread_join( sniffer_thread , NULL);
            log_log(LOG_DEBUG, "ServerSocket: Handler assigned");
        }
    }

    if (client_socket < 0)
    {
        log_log(LOG_ERROR, "ServerSocket: Accept failed");
        return 1;
    }
}

int server_socket_waitclients(server_socket *server, int *handler(socket_client_context *), void *data) {
    log_log(LOG_DEBUG, "ServerSocket: Waiting for incoming connections...");
    int c = sizeof(struct sockaddr_in);
    int client_socket;
    struct sockaddr_in client;
    socket_client_context *ctx;

    while (1 && server->do_close==0) {
        client_socket = accept(server->socket, (struct sockaddr *)&client, (socklen_t*)&c);
        log_log(LOG_DEBUG, "ServerSocket: Connection accepted");

        ctx = malloc(sizeof(socket_client_context));
        ctx->socket = client_socket;
        ctx->data = data;
        *handler(ctx);
    }

    if (client_socket < 0)
    {
        log_log(LOG_ERROR, "ServerSocket: Accept failed");
        return 1;
    }
}

void server_socket_close(server_socket *server) {
    log_log(LOG_DEBUG, "ServerSocket: Closing server");

    close(server->socket);
    log_log(LOG_DEBUG, "ServerSocket: Closed");
}
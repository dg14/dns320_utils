// main.c

#include "proto.h"
#include "socket.h"
#include <stdlib.h>
#include <signal.h>
#include "log.h"

server_socket server;

int *handler(socket_client_context *ctx) {
    dns320_connection *con;
    con = ctx->data;
    int sock = ctx->socket, read_size;
    log_log(LOG_DEBUG, "handler: Manage client connection");

    char *message, client_message[2000];
    message = "Greetings! I am your connection handler\n";

    write(sock , message , str_len(message));
    //Receive a message from client
    while ( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //write(sock , client_message , str_len(client_message));
        log_log(LOG_INFO, "Read bytes:%d, Message[%s]\n", read_size, client_message);
        if (read_size > 2) {
            // the 2 end bytes are '\r\n';
            client_message[read_size - 1] = '\0';
            client_message[read_size - 2] = '\0';
            const char *client_message_cleaned = (char *)str_trim(client_message);
            log_log(LOG_INFO, "bytes:%d, Message[%s]\n", str_len(client_message_cleaned), client_message_cleaned);

            dns320_return ret = dns320_manage_cmd(con, client_message_cleaned);
            if (ret.ret == 3) {
                log_log(LOG_DEBUG, "handler: Closing client");
                close(sock);
                return 0;
                //break;
            }
            if (ret.ret == 4) {
                log_log(LOG_DEBUG, "handler: Closing client and shutdown server");
                close(sock);
                //server_socket_close(&server);
                server.do_close = 1;
                break;
            }
            write(sock, ret.data, str_len(ret.data));
        } else {
            write(sock, "Messaggio non riconosciuto\n", str_len("Messaggio non riconosciuto\n"));
        }

    }
    free(ctx);
    return 0;

}

static void sighandler(int sig)
{
    syslog(LOG_DEBUG, "Signal Handler called\n");
    switch (sig)
    {
    case SIGINT:
        //cleanup(0, ls, 1);
        log_log(LOG_DEBUG, "Received SIGINT: Closing server");
        server_socket_close(&server);
        exit(EXIT_SUCCESS);
        break;
    case SIGTERM:
        /*        cleanup(0, ls, 1);
                if(stDaemonConfig.syncOnShutdown)
                    HandleCommand("systohc", 7, NULL, 0);
                syslog(LOG_INFO, "Shutting down machine in %d seconds...\n", stDaemonConfig.delayShutdown);
                DeviceShutdownCmd[5] = (char)stDaemonConfig.delayShutdown;
                SendCommand(fd, DeviceShutdownCmd, 0);
        */
        log_log(LOG_DEBUG, "Received SIGTERM.");
        exit(EXIT_SUCCESS);
        break;
    }
}



int main(int argc, char *argv[]) {
    log_set_level(LOG_ALL);
    dns320_connection con;

    // Register some signal handlers
    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);

    //dns320_init (&con, "/dev/ttyS1",15200);

    log_log(LOG_INFO, "Opening server at 5000/tcp");

    server_socket_open(&server, 5000);
    log_log(LOG_INFO, "Listening");
    server_socket_listen(&server, 4);

    log_log(LOG_INFO, "Wait clients");
    server_socket_waitclients(&server, handler, &con);

    log_log(LOG_INFO, "Closing server");
    server_socket_close(&server);

    //dns320_close(&con);
}



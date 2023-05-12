#include "../include/constants.h"
#include "../include/client.h"
#include "../include/game.h"
#include "../include/memory_controller.h"
#include "../include/message.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int network_is_connection_down(int socket)
{
    int error = 0;
    socklen_t length = sizeof(error);
    int state = getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, &length);

    if (state != 0) {
        printf("Error getting socket error code: %s\n", strerror(state));
        return 1;
    }

    if (error != 0) {
        printf("Socket error: %s\n", strerror(error));
        return 1;
    }

    return 0;
}

void network_process_client(int client_id, int client_socket)
{
    printf("Processing client id=%d, socket=%d\n", client_id, client_socket);
    client_prejoin(client_id);

    while(1) {
        struct Message msg;

        // Check if connections is still up
        if (network_is_connection_down(client_socket) > 0) {
            // Cleanup;
            printf("Disconnecting client id=%d, socket=%d\n", client_id, client_socket);
            client_leave(client_id);
            memory_decrease_client_count();
            close(client_socket);
            return;
        }

        printf("Network pulse #1 for %d\n", client_id);
        // Check if there's anything to read
        ssize_t msg_length = recv(client_socket, &msg, sizeof(msg), MSG_PEEK | MSG_DONTWAIT);
        printf("Network pulse #2 for %d\n", client_id);

        if (msg_length >= sizeof(struct Message)) {
            printf("Network pulse #2.5 for %d\n", client_id);
            recv(client_socket, &msg, sizeof(msg), 0);
            message_print(&msg);
            client_set_incoming_message(client_id, &msg);
        }

        printf("Network pulse #3 for %d\n", client_id);

        // Check if there's anything to read
        if (client_has_outgoing_message(client_id) == 1) { 
            printf("Network pulse #3.5 for %d\n", client_id);       
            client_pull_outgoing_message(client_id, &msg);
            send(client_socket, &msg, sizeof(msg), 0);
        }

        printf("Network pulse #4 for %d\n", client_id);
    }
}

void network_deny_join(int client_socket, const char* reason)
{
    struct Message msg;
    message_encode(&msg, "MESG", 0, reason);
    send(client_socket, &msg, sizeof(msg), 0);
}

void network_accept_client(int main_socket) {
    struct sockaddr_in client_address;
    int address_size = sizeof(client_address);
    int client_socket = -1, c_pid = 0;
    
    int new_client_id;
    client_socket = accept(main_socket, (struct sockaddr*)&client_address, (socklen_t*)&address_size);
    
    if (client_socket < 0) {
        printf("Client accept failure. Errno: %d\n", errno);
        return;
    }
    printf("New client connection (%s) incoming.\n", inet_ntoa(client_address.sin_addr));

    new_client_id = game_new_client_id();

    if (new_client_id < 0) {
        network_deny_join(client_socket, "Server is full");
        close(client_socket);
        return;
    }
    memory_increase_client_count();    

    c_pid = fork();

    if (c_pid == 0) {
        close(main_socket);
        c_pid = fork();

        if (c_pid == 0) {
            network_process_client(new_client_id, client_socket);
            exit(0);
        } else {
            wait(NULL);
            printf("Successfully orphaned client %d\n", new_client_id);
            exit(0);
        }
    } else {
        close(client_socket);
    }
}

void network_start(int port)
{
    printf("Network starting.\n");
    
    struct sockaddr_in address;
    int main_socket;
    
    main_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (main_socket < 0) {
        perror("Error opening main socket.\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if ( bind(main_socket, (struct sockaddr*)&address, sizeof(address)) < 0 ) {
        perror("Port bind failed.\n");
        exit(EXIT_FAILURE);
    }

    if ( listen(main_socket, MAX_CLIENTS) < 0 ) {
        perror("Listen failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d.\n", port);

    while (1) {
        network_accept_client(main_socket);
    }
    
    shutdown(main_socket, SHUT_RDWR);
}


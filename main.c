
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define LISTEN_BACKLOG 5
#define MAX_NUM_CONNECTIONS 10
#define RECV_BUF_SIZE 128


void run_server_program(int port);
int setup_server_listen_socket(int port);

int main() {
    printf("Hello world\n");

    run_server_program(12345);
}

void run_server_program(int port) {
    int listen_fd = setup_server_listen_socket(port);

    int connections[MAX_NUM_CONNECTIONS];
    int number_of_connections = 0;

    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(listen_fd, &fds);
        int max_fd = listen_fd;

        // Add all connections to fds
        for (int i = 0; i < number_of_connections; i++) {
            int current_fd = connections[i];
            FD_SET(current_fd, &fds);
            if (max_fd < current_fd) {
                max_fd = current_fd;
            }
        }

        if (select(max_fd + 1, &fds, NULL, NULL, NULL) < 0) {
            perror("select() failed.");
            exit(1);
        }

        // Accept a new client
        if (FD_ISSET(listen_fd, &fds) && 
            number_of_connections < MAX_NUM_CONNECTIONS) {
            int client_fd = accept(listen_fd, NULL, 0);
            connections[number_of_connections] = client_fd;
            number_of_connections++;

            printf("Accepted a client\n");
        }

        // Handle received data
        for (int i = 0; i < number_of_connections; i++) {
            int current_fd = connections[i];

            if (FD_ISSET(current_fd, &fds)) {
                char data[RECV_BUF_SIZE];

                int len = recv(current_fd, data, RECV_BUF_SIZE, 0);

                if (len < 0) {
                    perror("Error with recv().");
                    exit(1);
                }

                if (len == 0) {
                    // TODO: Close connection
                }

                data[len] = 0;

                printf("%s\n", data);
            }
        }
    }
}

int setup_server_listen_socket(int port) {
    // Create the socket file descriptor
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        perror("socket() failed.");
        exit(1);
    }

    // Bind the socket to the specified port
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind() failed.");
        exit(1);
    }

    // Starting listening
    if (listen(fd, LISTEN_BACKLOG) < 0) {
        perror("listen() failed.");
        exit(1);
    }

    return fd;
}
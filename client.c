
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include "constants.h"


int connect_to_server(struct sockaddr_in server_sa);

void run_client_program(struct sockaddr_in server_sa) {
    int server_fd = connect_to_server(server_sa);

    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(server_fd, &fds);
        FD_SET(STDIN_FILENO, &fds);

        int max_fd = server_fd;
        if (STDIN_FILENO > max_fd) {
            max_fd = STDIN_FILENO;
        }

        if (select(max_fd + 1, &fds, NULL, NULL, NULL) < 0) {
            perror("select() failed");
            exit(1);
        }

        if (FD_ISSET(server_fd, &fds)) {
            char data[RECV_BUF_SIZE];

            int len = recv(server_fd, data, RECV_BUF_SIZE, 0);

            if (len < 0) {
                perror("Error with recv()");
                exit(1);
            }

            if (len == 0) {
                // TODO: Close connection
            }

            data[len] = 0;

            printf("%s", data);
        }

        if (FD_ISSET(STDIN_FILENO, &fds)) {
            char data[RECV_BUF_SIZE];

            int len = read(STDIN_FILENO, data, RECV_BUF_SIZE);

            if (len < 0) {
                perror("Error with read()");
                exit(1);
            }

            data[len] = 0;

            ssize_t send_len = send(server_fd, data, len, 0);

            if (send_len < 0) {
                perror("Error with send().");
                exit(1);
            }

            if (send_len == 0) {
                // TODO: Close connection
            }
        }
    }
}

int connect_to_server(struct sockaddr_in server_sa) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        perror("socket() failed");
        exit(1);
    }

    if (connect(fd, (struct sockaddr *)&server_sa, sizeof(server_sa)) < 0) {
        perror("connect() failed");
        exit(1);
    }

    return fd;
}
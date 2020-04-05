
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include "constants.h"


struct client {
    int server_fd;
    int max_fd;
    fd_set default_fds;
    fd_set fds;
};

static struct client create_client(struct sockaddr_in server_sa);
static int connect_to_server(struct sockaddr_in server_sa);
static void wait_on_select_and_update_fds(struct client *self);
static void handle_incoming_data(struct client *self);
static void handle_user_input(struct client *self);


void run_client_program(struct sockaddr_in server_sa) {
    struct client cli = create_client(server_sa);

    while (1) {
        wait_on_select_and_update_fds(&cli);
        handle_incoming_data(&cli);
        handle_user_input(&cli);
    }
}

static struct client create_client(struct sockaddr_in server_sa) {
    struct client cli;
    cli.server_fd = connect_to_server(server_sa);

    cli.max_fd = cli.server_fd;
    if (STDIN_FILENO > cli.max_fd) {
        cli.max_fd = STDIN_FILENO;
    }

    FD_ZERO(&cli.default_fds);
    FD_SET(cli.server_fd, &cli.default_fds);
    FD_SET(STDIN_FILENO, &cli.default_fds);

    FD_ZERO(&cli.fds);

    return cli;
}

static int connect_to_server(struct sockaddr_in server_sa) {
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

static void wait_on_select_and_update_fds(struct client *self) {
    self->fds = self->default_fds;

    if (select(self->max_fd + 1, &self->fds, NULL, NULL, NULL) < 0) {
        perror("select() failed");
        exit(1);
    }
}

static void handle_incoming_data(struct client *self) {
    if (FD_ISSET(self->server_fd, &self->fds)) {
        char data[MSG_BUF_SIZE];

        int len = recv(self->server_fd, data, MSG_BUF_SIZE, 0);

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
}

static void handle_user_input(struct client *self) {
    if (FD_ISSET(STDIN_FILENO, &self->fds)) {
        char data[MSG_BUF_SIZE];

        int len = read(STDIN_FILENO, data, MSG_BUF_SIZE);

        if (len < 0) {
            perror("Error with read()");
            exit(1);
        }

        data[len] = 0;

        ssize_t send_len = send(self->server_fd, data, len, 0);

        if (send_len < 0) {
            perror("Error with send().");
            exit(1);
        }

        if (send_len == 0) {
            // TODO: Close connection
        }
    }
}
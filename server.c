
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include "constants.h"
#include "list.h"


struct server {
    int listen_fd;

    struct list client_fds;

    int max_fd;
    fd_set fds_before_select;
    fd_set fds_after_select;
};

static struct server create_server(int port);
static int setup_listen_socket(int port);
static void wait_on_select_and_update_fds(struct server *self);
static void accept_client(struct server *self);
static void handle_incoming_data(struct server *self);
static void close_connection_for_index(struct server *self, int index);
static void send_to_all_except(struct server *self, char data[], int len, int except_index);


void run_server_program(int port) {
    struct server serv = create_server(port);

    while (1) {
        wait_on_select_and_update_fds(&serv);
        accept_client(&serv);
        handle_incoming_data(&serv);
    }
}

static struct server create_server(int port) {
    struct server serv;

    serv.listen_fd = setup_listen_socket(port);
    serv.max_fd = serv.listen_fd;

    FD_ZERO(&serv.fds_before_select);
    FD_SET(serv.listen_fd, &serv.fds_before_select);

    return serv;
}

static int setup_listen_socket(int port) {
    // Create the socket file descriptor
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        perror("socket() failed");
        exit(1);
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("setsockopt() failed");
        exit(1);
    }

    // Bind the socket to the specified port
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = port;

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

static void wait_on_select_and_update_fds(struct server *self) {
    self->fds_after_select = self->fds_before_select;

    if (select(self->max_fd + 1, &self->fds_after_select, NULL, NULL, NULL) < 0) {
        perror("select() failed.");
        exit(1);
    }
}

static void accept_client(struct server *self) {
    if (FD_ISSET(self->listen_fd, &self->fds_after_select)) {
        int client_fd = accept(self->listen_fd, NULL, 0);

        if (client_fd < 0) {
            perror("Errow with accept()");
            exit(1);
        }

        append(&self->client_fds, client_fd);
        
        if (client_fd > self->max_fd) {
            self->max_fd = client_fd;
            FD_SET(client_fd, &self->fds_before_select);
        }

        printf("Accepted a client\n");
    }
}

static void handle_incoming_data(struct server *self) {
    for (int i = 0; i < size(&self->client_fds); i++) {
        int current_fd = element_at(&self->client_fds, i);

        if (FD_ISSET(current_fd, &self->fds_after_select)) {
            char data[MSG_BUF_SIZE+1];

            int len = recv(current_fd, data, MSG_BUF_SIZE, 0);
            data[len] = 0;

            if (len < 0) {
                perror("Error with recv().");
                exit(1);
            } else if (len == 0) {
                close_connection_for_index(self, i);
            } else {
                send_to_all_except(self, data, len, i);
            }
        }
    }
}

static void close_connection_for_index(struct server *self, int index) {
    delete_element_at(&self->client_fds, index);

    self->max_fd = self->listen_fd;

    FD_ZERO(&self->fds_before_select);
    FD_SET(self->listen_fd, &self->fds_before_select);

    for (int i = 0; i < size(&self->client_fds); i++) {
        int current_fd = element_at(&self->client_fds, i);
        
        if (current_fd > self->max_fd) {
            self->max_fd = current_fd;
        }

        FD_SET(current_fd, &self->fds_before_select);
    }

    printf("Closed a connection\n");
}

static void send_to_all_except(struct server *self, char data[], int len, int except_index) {
    for (int i = 0; i < size(&self->client_fds); i++) {
        if (i == except_index) {
            continue;
        }

        int current_fd = element_at(&self->client_fds, i);
        ssize_t send_len = send(current_fd, data, len, 0);

        if (send_len < 0) {
            perror("Error with send().");
            exit(1);
        }

        if (send_len == 0) {
            close_connection_for_index(self, i);
            i--; // To compensate for the deleted fd in the list
        }
    }
}
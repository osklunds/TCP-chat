
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LISTEN_BACKLOG 5


int setup_server_listen_socket(int port);

int main() {
    printf("Hello world\n");

    int fd = setup_server_listen_socket(12345);
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
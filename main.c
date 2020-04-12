
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "client.h"
#include "list.h"


int main(int argc, char *argv[]) {
    struct list lst = create_list();
    insert(&lst, 2);

    printf("%d\n", size(&lst));

    return 0;

    if (argc == 2) {
        char *port_string = argv[1];
        int port = strtol(port_string, NULL, 10);
        port = htons(port);

        run_server_program(port);
    } else if (argc == 4) {
        struct sockaddr_in sa;

        char *ip_string = argv[1];
        inet_pton(AF_INET, ip_string, &(sa.sin_addr));

        char *port_string = argv[2];
        int port = strtol(port_string, NULL, 10);
        port = htons(port);
        sa.sin_port = port;

        char *name = argv[3];

        run_client_program(sa, name);
    } else {
        printf("Invalid number of arguments.\n");
        exit(1);
    }
}
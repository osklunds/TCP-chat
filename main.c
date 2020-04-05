
#include <stdio.h>
#include <stdlib.h>

#include "server.h"


int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *port_string = argv[1];
        int port = strtol(port_string, NULL, 10);

        run_server_program(port);
    } else if (argc == 3) {

    } else {
        printf("Invalid number of arguments.\n");
        exit(1);
    }
}
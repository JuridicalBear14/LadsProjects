#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "defn.h"

int client_fd;
char* name;
int namelen;

void* send_m(void* argv) {
    char buf[MAXMSG];

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        // Remove newline
        buf[strlen(buf) - 1] = 0;

        send(client_fd, buf, sizeof(buf), 0);
        memset(buf, 0, sizeof(buf));
    }
}

void* recieve(void* argv) {
    char buf[MAXMSG];

    while (read(client_fd, buf, MAXMSG) != 0) {
        fprintf(stdout, "%s\n", buf);
        memset(buf, 0, sizeof(buf));
    }
}

// Init function run on connection
void init() {
    // Send name over
    send(client_fd, name, NAMELEN, 0);
}

int main(int argc, char** argv) {
    char* ip = "127.0.0.1";
    name = "NULL";

    // Name given
    if (argc > 1) {
        name = argv[1];
    }

    // ip given
    if (argc > 2) {
        ip = argv[2];
    }

    int status, valread;
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    init();

    pthread_t listener;
    pthread_t sender;

    pthread_create(&listener, NULL, recieve, NULL);
    pthread_create(&sender, NULL, send_m, NULL);

    // Cleanup threads
    pthread_join(listener, NULL);
    pthread_join(sender, NULL);

    // closing the connected socket
    close(client_fd);
    return 0;
}

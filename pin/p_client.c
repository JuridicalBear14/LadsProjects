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

void* send_m(void* argv) {
    char buf[1024];

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        send(client_fd, buf, sizeof(buf), 0);
        memset(buf, 0, sizeof(buf));
    }
}

void* recieve(void* argv) {
    char buf[1024];

    while (read(client_fd, buf, 1024) != 0) {
        fprintf(stderr, "Recieved -> %s", buf);
        memset(buf, 0, sizeof(buf));
    }
}

int main(int argc, char** argv) {
    char* ip = "127.0.0.1";

    // ip given
    if (argc > 1) {
        ip = argv[1];
    }

    int status, valread;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
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

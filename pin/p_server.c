#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include "defn.h"

// Array of fds to monitor
struct pollfd pollfds[MAXUSR];

// Find next open port index
int nextindex() {
    for (int i = 0; i < MAXUSR; i++) {
        if (pollfds[i].fd == 0) {
            return i;
        }
    }

    // No open slots
    return -1;
}

// Send message to all used ports other than given
void sendall(int ix, char* buf) {
    for (int i = 0; i < MAXUSR; i++) {
        if (pollfds[i].fd != 0 && i != ix) {
            // Send to socket
            write(pollfds[i].fd, buf, strlen(buf));
        }
    }
}

// Initialization function called on every new connection
void init(int ix) {

}

// Monitor and relay socket messages
void* relay(void* argv) {
    int n;
    char buf[1024];

    // Wait for event
    while ((n = poll(pollfds, MAXUSR, 5)) != -1) {

        // Figure out which fd(s) updated
        for (int i = 0; i < MAXUSR; i++) {
            if (pollfds[i].revents & POLLIN) {

                // Ready to read
                if (!read(pollfds[i].fd, buf, sizeof(buf))) {
                    // Closed
                    close(pollfds[i].fd);
                    pollfds[i].fd = 0;

                    fprintf(stderr, "Closed slot: %d\n", i);
                    continue;
                }

                fprintf(stderr, "Message recieved from slot: %d\n", i);

                // Send message to all others
                sendall(i, buf);
                memset(buf, 0, sizeof(buf));

                // Reset revents
                pollfds[i].revents = 0;
            }
        }
    }
}

// Main startup and port accept function
int main(void) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    fprintf(stderr, "Starting server...\n");

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
       perror("setsockopt");
       exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 5555
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAXUSR) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Set up pollfds
    for (int i = 0; i < MAXUSR; i++) {
        pollfds[i].events = POLLIN;
    }

    // Set up relay thread
    pthread_t r_thread;
    pthread_create(&r_thread, NULL, relay, NULL);

    // Wait and accept incoming connections
    int index;
    while (1) {
        if ((index = nextindex()) == -1) {
            // No open slots
            continue;
        }

        fprintf(stderr, "Slot %d available\n", index);

        // Accept connection
        if ((pollfds[index].fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "Connection accepted in slot %d, fd: %d\n", index, pollfds[index].fd);
        init(index);
    }

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}

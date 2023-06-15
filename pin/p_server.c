#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include "defn.h"

// Array of fds to monitor
struct pollfd pollfds[MAXUSR];

// Monitor and relay socket messages
void* relay(void* argv) {
    int n;
    char buf[1024];

    // Wait for event
    while ((n = poll(pollfds, MAXUSR, -1)) != -1) {

        // Figure out which fd(s) updated
        for (int i = 0; i < MAXUSR; i++) {
            if (pollfds[i].revents & POLLIN) {
                // Ready to read
                if (!read(pollfds[i].fd, buf, sizeof(buf))) {
                    // Closed
                    close(pollfds[0].fd);
                    close(pollfds[1].fd);
                    return NULL;
                }

                // Reset revents
                pollfds[i].revents = 0;

                // Send to other socket
                write(pollfds[!i].fd, buf, strlen(buf));
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
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Set up pollfds
    pollfds[0].events = POLLIN;
    pollfds[1].events = POLLIN;

    if ((pollfds[0].fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if ((pollfds[1].fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    relay(NULL);

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}

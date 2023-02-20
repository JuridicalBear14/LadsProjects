#include <czmq.h>

int main() {
    printf("Starting server...\n");
    // Make responder socket
    zsock_t* responder = zsock_new(ZMQ_REP);

    // Bind socket to port 5555 (star for localhost)
    int r = zsock_bind(responder, "tcp://*:5555");
    
    // Check if bind worked
    if (r != 5555) {
        printf("Failed to bind to port");
    }

    // Event loop
    while (true) {

        // Recieve message
        char* msg = zstr_recv(responder);
    
        if (strcmp(msg, "quit\n")) {
            printf("%s: %ld\n", msg + 8, strlen(msg));
    
            // Send back message
            zstr_send(responder, "Recieved!");
        } else {
            // Quit server
            printf("Stopping server\n");
            zstr_send(responder, "quitting");

            zstr_free(&msg);
            break;
        }

        // Free message
        zstr_free(&msg);
    }
    zsock_destroy(&responder);
}

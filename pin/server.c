#include <czmq.h>

int main() {

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

        if (!strcmp(msg, "Hello")) {
            printf("%s", msg);

            // Send back message
            zstr_send(responder, "World");
        }

        sleep(1);

        // Free message
        zstr_free(&msg);
    }
    zsock_destroy(&responder);
}

#include <czmq.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// Default assignment port
#define defport 5500
#define maxusers 2

typedef struct {
    char* txtid;  // User's text id
    int uid;   // User's integer id
    int port;
    char* outbound;   // Outgoing message for given user
} user_t;

// Master thread shutoff control
bool shutoff = false;

/* Look up and return the port number for a given user id */
int lookup(zsock_t** connections, char* id) {

}


/* Handles routing for sending message to specific user */
void send_message(char* dest, char* msg) {

}

/* Incoming message listener for a given socket*/
void m_listener() {

}

/* Assign incoming user connection to next open port
    and start a listener, returns assigned port */
int port_assign(zsock_t** connections, char* msg) {

}

/* Main event loop, effectively main function, but seperated
    to allow command line I/O to be parent thread */
static void* control_loop() {
    printf("Starting server...\n");

    // Init assignment socket
    zsock_t* assignment = zsock_new(ZMQ_REP);

    char* port = (char*) malloc(sizeof(char) * 20);
    sprintf(port, "tcp://*:%d", defport);
    printf("Connecting to port: %s\n", port);
    // Bind to default port
    int r = zsock_bind(assignment, "tcp://*:5500");
    free(port);  // No longer needed

    if (r != defport) {
        printf("Failed to bind to port: %d\n", defport);
    }

    // Initialize connection array
    zsock_t** connections = malloc(sizeof(zsock_t*) * maxusers);

    // Incoming connection listener
    while (true) {
        char* msg = zstr_recv(assignment);

        // Check for shutoff
        if (!strcmp(msg, "quit\n")) {
            zstr_send(assignment, "Server-shutoff");
            zstr_free(&msg);
            printf("Shutoff\n");
            break;
        }

        // Recieved port assignment request
        if (!strcmp(msg + 8, "PA-REQ")) {
            // Assign a port and send back assignment
            int port = port_assign(connections, msg);

            // Convert to string
            char str[5];
            sprintf(str, "%d", port);

            zstr_send(assignment, str);

        } else {
            printf("Unkown inbound: %s\n", msg);
            zstr_send(assignment, "err");
        }

        zstr_free(&msg);
    }

    // Remember to free all open ports
    zsock_destroy(&assignment);
}

// Creates a socket to send exit signal to control loop
void cancel_ctrl_loop() {
    zsock_t* cancel = zsock_new(ZMQ_REQ);

    zsock_connect(cancel, "tcp://localhost:5500");

    zstr_send(cancel, "quit\n");
    printf("Sent\n");
    zsock_destroy(&cancel);
}

int main() {
    printf("Initializing server\n");

    // Start control loop
    pthread_t pid;
    pthread_create(&pid, NULL, &control_loop, NULL);

    sleep(5);

    shutoff = true;
    cancel_ctrl_loop();

    // Give threads some time to cleanup before exit
    pthread_join(pid, NULL);
    exit(0);
}

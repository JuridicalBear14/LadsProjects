#include <czmq.h>
#include <unistd.h>

int main() {

    // Make requester socket
    zsock_t* requester = zsock_new(ZMQ_REQ);

    // Connect to port 5555
    zsock_connect(requester, "tcp://localhost:5555");

    // Send string
    zstr_send(requester, "Hello");

    // Wait for response
    sleep(1);

    // Recieve string
    char* str = zstr_recv(requester);

    printf("%s\n", str);

    zsock_destroy(&requester);
}

#include <czmq.h>
#include <unistd.h>
#include <sys/select.h>

/*Function to read line into an array and return a pointer
* to a percectly sized array*/
char *readline() {
    char line[500]; //Variable to hold line before size is known
    int i; //Iteration variable

    //Fills line with all characters
    for (i = 0; (line[i] = getchar()) != '\n'; i++);

    line[i] = '\0'; //Add null char to end of line

    //Create a variable the size of the actual input data and copy to it
    char *final = malloc(i + 1);
    strcpy(final, line);

    return final;
}

int main() {
    // Set up variables for non blocking I/O
    fd_set rfds;
    struct timeval tv;
    int retval;

    // Watch stdin
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    // Wait up to 1 second
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    // Make requester socket
    zsock_t* requester = zsock_new(ZMQ_DEALER);

    // Connect to port 5555
    //zsock_connect(requester, "tcp://10.111.172.24:5555");
    zsock_connect(requester, "tcp://localhost:5555");

    char* str = "tets";
    char* rep;

    while (true) {
        zstr_send(requester, str);

        sleep(3);

        zstr_free(&rep);
    }
    zsock_destroy(&requester);
}

#include <czmq.h>
#include <unistd.h>

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

    // Make requester socket
    zsock_t* requester = zsock_new(ZMQ_REQ);

    // Connect to port 5555
    zsock_connect(requester, "tcp://10.111.172.24:5555");
    //zsock_connect(requester, "tcp://localhost:5555");

    char* str;
    char* rep;

    while (true) {
        // Get user message or response or both
        str = readline();
        if ((rep = zstr_recv(requester)) != NULL){
            // If there's a response, print it
            printf("%s", rep);
        }

        if (!strcmp(str, "exit")) {
            printf("exiting\n");
            //zstr_send(requester, "Exit");
            break;
        }

        zstr_send(requester, str);

        zstr_free(&rep);
        free(str);
    }
    zsock_destroy(&requester);
}

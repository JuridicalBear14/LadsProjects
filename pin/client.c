#include <czmq.h>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>

char* str;

void* readin(void* str) {
    char buff[100];

    while (fgets(buff, 100, stdin) != NULL) {
        str = buff;
    }

}

int main() {
    // Make requester socket
    zsock_t* requester = zsock_new(ZMQ_REQ);

    // Connect to port 5555
    //zsock_connect(requester, "tcp://10.111.172.24:5555");
    zsock_connect(requester, "tcp://localhost:5555");

    char* rep;

    pthread_t pid;
    pthread_create(&pid, NULL, readin, NULL);

    while (true) {
        zstr_send(requester, str);
        //str = NULL;

        rep = zstr_recv(requester);
        //printf("%s\n", rep);

        zstr_free(&rep);
    }

    pthread_join(pid, NULL);
    zsock_destroy(&requester);
}

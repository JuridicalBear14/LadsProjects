#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <linux/in.h>

#define PORT 5555

void error(void) {
    fprintf(stderr, "Error!\n");
    exit(0);
}

int main(void) {
    int fd, new;
    struct socketaddr addr;

    
    if(fd = socket(AF_LOCAL, SOCK_STREAM, 0) < 0) {error();}

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(fd, &addr, sizeof(addr)) < 0) {error();}

    listen(fd, 3);

    if (new = accept(fd, &addr, sizeof(addr)) < 0) {error();}

    // Now ready to go


    // Shutdown
    close(new);

    shutdown(fd, SHUT_RDWR);

    return 0;
}
#include <stdio.h>
#include <unistd.h>

#include "sock5.h"

int
main(int argc, char **argv) {
    int listenfd; 

    printf("hello world!\n");
    listenfd = tcp("127.0.0.1", 8080);
    run_epoll(listenfd);
    close(listenfd);

    return 0;
}

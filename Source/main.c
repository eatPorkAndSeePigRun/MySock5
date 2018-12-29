#include <stdio.h>

#include "sock5.h"

int
main(int argc, char **argv) {
    int listenfd; 

    printf("hello world!\n");
    listenfd = tcp("127.0.0.1", 8080);

    return 0;
}

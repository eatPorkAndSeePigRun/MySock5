#include <netinet/in.h>
#include <strings.h>
#include <apra/inet.h>

#include "sock5.h"
#include "wrap.h"

extern int 
tcp(const char *ip, uint16_t port) {
    int listenfd;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    listenfd = Socket(AF_INET, SOCK_STREAM);
    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    Listen(listenfd, 5);
}

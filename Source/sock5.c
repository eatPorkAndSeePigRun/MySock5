#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <sys/epoll.h>

#include "sock5.h"
#include "wrap.h"

extern BOOL
sock5_license(void *buf) {
    client_license_request *license_request = NULL;

    license_request = (client_license_request *) buf;
    if (license_request->ver != 0x05)   // 协议版本错误
        return FALSE;

    bzero(buf, sizeof(buf));
    server_license_response license_response = {0x05, 0x00};
    memcpy(buf, &license_response, sizeof(license_response));
    return TRUE;
}

extern BOOL
sock5_connect(void *buf, int *dest_socketfd) {
    client_connect_request *connect_request = NULL;

    connect_request = (client_connect_request *) buf;
    if (connect_request->ver != 0x05)   // 协议版本错误
        return FALSE;
    if (connect_request->cmd != 0x01)   // 请求命令错误，非TCP
        return FALSE;
    if (connect_request->type != 0x01)  // 请求类型错误，非IPv4
        return FALSE;

    *dest_socketfd = proxy_socket((void *) &(connect_request->addr), (void *) &(connect_request->port));
    bzero(buf, sizeof(buf));
    server_connect_response connect_response = {0x05, 0x00, 0x00, 0x01};
    memcpy(buf, &connect_response, sizeof(connect_response));
    return TRUE;
}

static int
proxy_socket(const struct in_addr *ip, const in_port_t *port) {
    int proxy_sockfd;
    struct sockaddr_in proxy_servaddr;

    bzero(&proxy_servaddr, sizeof(proxy_servaddr));
    proxy_servaddr.sin_family = AF_INET;
    proxy_servaddr.sin_addr = *ip;
    proxy_servaddr.sin_port = *port;

    proxy_sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    nonblock_connect(proxy_sockfd, (struct sockaddr *) &proxy_servaddr, sizeof(proxy_servaddr));
    return proxy_sockfd;
}

extern void
forward(int epollfd, int fd) {
    struct epoll_event ev;

    ev.events = EPOLLOUT;
    ev.data.fd = fd;
    Epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

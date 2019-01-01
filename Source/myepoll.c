#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>

#include "wrap.h"
#include "myepoll.h"
#include "sock5.h"


extern int
tcp(const char *ip, uint16_t port) {
    int listenfd;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    Listen(listenfd, 5);
    return listenfd;
}

extern void
run_epoll(int listenfd) {
    int epollfd, nfds;
    struct epoll_event ev, events[MAX_EPOLL_EVENTS];
    int i;
    char buf[BUF_SIZE];
    bzero(buf, sizeof(buf));
    bzero(client_status, sizeof(client_status));
    bzero(dest_socket, sizeof(dest_socket));
    bzero(client_socket, sizeof(client_socket));


    epollfd = Epoll_create(EPOLL_SIZE);

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    Epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

    while (1) {
        nfds = Epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listenfd)
                handle_accept(epollfd, listenfd);
            else if (events[i].events == EPOLLIN)
                do_read(epollfd, events[i].data.fd, buf);
            else if (events[i].events == EPOLLOUT)
                do_write(epollfd, events[i].data.fd, buf);
        }
    }
}

static void
handle_accept(int epollfd, int listenfd) {
    int clientfd;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen;
    struct epoll_event ev;

    clientaddrlen = sizeof(clientaddr);
    clientfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddrlen);

    ev.events = EPOLLIN;
    ev.data.fd = clientfd;
    Epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);

    client_status[clientfd] = STATUS_INIT;
}

static void
do_read(int epollfd, int fd, void *buf) {
    ssize_t n;
    struct epoll_event ev;

    n = Read(fd, buf, BUF_SIZE);
    if (n == 0) {
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        Epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
        close(fd);
        client_socket[dest_socket[fd]] = STATUS_UNINIT;
        dest_socket[fd] = STATUS_UNINIT;
        client_status[fd] = STATUS_UNINIT;
        return;
    }

    run_sock5(epollfd, fd, buf);
}

static void
run_sock5(int epollfd, int fd, void *buf) {
    struct epoll_event ev;
    int dest_socketfd;

    // 若fd是目标服务器
    if (client_socket[fd] != STATUS_UNINIT) {
        forward(epollfd, client_socket[fd]);
    }
    // 若fd是客户端
    if (client_status[fd] == STATUS_INIT) {               // sock5请求 
        if (!sock5_license(buf))
            return;
        client_status[fd] = STATUS_LICENSE;
        forward(epollfd, fd);
    } else if (client_status[fd] == STATUS_LICENSE) {     // sock5连接
        if (!sock5_connect(buf, &dest_socketfd))
            return;
        client_status[fd] = STATUS_CONNECT;
        forward(epollfd, fd);

        dest_socket[fd] = dest_socketfd;    // 用int[]当作map，clientfd对应destfd
        client_socket[dest_socketfd] = fd;
        ev.events = EPOLLIN;                // 把dest添加到epollfd
        ev.data.fd = dest_socketfd;
        Epoll_ctl(epollfd, EPOLL_CTL_ADD, dest_socketfd, &ev);
    } else if (client_status[fd] == STATUS_CONNECT) {     // sock5转发
        forward(epollfd, dest_socket[fd]);
    }
}

static void
do_write(int epollfd, int fd, void *buf) {
    struct epoll_event ev;

    Write(fd, buf, sizeof(buf));
    bzero(buf, BUF_SIZE);
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    Epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
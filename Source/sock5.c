#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string.h>

#include "sock5.h"
#include "wrap.h"

#include <stdio.h>

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
}

extern void
run_epoll(int listenfd) {
    int epollfd, nfds;
    struct epoll_event ev, events[MAX_EPOLL_EVENTS];
    int i;
    char buf[BUF_SIZE];
    bzero(buf, BUF_SIZE);

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

extern void
handle_accept(int epollfd, int listenfd) {
    int clientfd;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen;
    struct epoll_event ev;

    clientfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddrlen);

    ev.events = EPOLLIN;
    ev.data.fd = clientfd;
    Epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
}

extern void
do_read(int epollfd, int fd, void *buf) {
    int n;
    struct epoll_event ev;

    n = Read(fd, buf, BUF_SIZE);
    if (n == 0) {
        close(fd);
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        Epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
    } else {
        ev.events = EPOLLOUT;
        ev.data.fd = fd;
        Epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
    }
    printf("read: %s\n", buf);
}

extern void
do_write(int epollfd, int fd, void *buf) {
    int n;
    struct epoll_event ev;

    n = Write(fd, buf, strlen(buf));
    printf("write: %s\n", buf);
    bzero(buf, BUF_SIZE);
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    Epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

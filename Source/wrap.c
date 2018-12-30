#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <stdio.h>

#include "wrap.h"

extern int
Socket(int family, int type, int protocol) {
    int fd;

    fd = socket(family, type, protocol);
    if (fd < 0) {
        perror("socket error");
        exit(1);
    }
    return fd;
}

extern void
Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    int n;

    n = bind(fd, sa, salen);
    if (n < 0) {
        perror("bind error");
        exit(1);
    }
}

extern void
Listen(int fd, int backlog) {
    int n;

    n = listen(fd, backlog);
    if (n < 0) {
        perror("listen error");
        exit(1);
    }
}

extern int
Accept(int listenfd, struct sockaddr *sa, socklen_t *salenptr) {
    int fd;

    fd = accept(listenfd, sa, salenptr);
    if (fd < 0) {
        perror("accept error");
        exit(1);
    }
    return fd;
}

extern void
Connect(int fd, const struct sockaddr *sa, socklen_t salen) {
    int n;

    n = connect(fd, sa, salen);
    if (n < 0) {
        perror("connect error");
        exit(1);
    }
}

extern ssize_t
Read(int fd, void *buf, size_t nbytes) {
    ssize_t n;

    while ((n = read(fd, buf, nbytes)) < 0) {
        if (errno == EINTR)
            continue;
        else {
            perror("read error");
            return -1;
        }
    }
    return n;
}

extern ssize_t
Write(int fd, void *buf, size_t nbytes) {
    ssize_t n;

    while ((n = write(fd, buf, nbytes)) < 0) {
        if (errno == EINTR)
            continue;
        else {
            perror("write error");
            return -1;
        }
    }
    return n;
}

extern int
Epoll_create(int size) {
    int epfd;

    epfd = epoll_create(size);
    if (epfd < 0) {
        perror("epoll_create error");
        exit(1);
    }
    return epfd;
}

extern int
Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
    int n;

    n = epoll_ctl(epfd, op, fd, event);
    if (n < 0) {
        perror("epoll_ctl error");
        exit(1);
    }
    return n;
}

extern int
Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout) {
    int nfds;

    nfds = epoll_wait(epfd, events, maxevents, timeout);
    if (nfds < 0) {
        perror("epoll_wait error");
        exit(1);
    }
    return nfds;
}
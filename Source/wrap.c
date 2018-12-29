#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include "wrap.h"

extern int
Socket(int family, int type, int protocol = 0) {
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
Accept(int fd, const struct sockaddr *sa, socklen_t *salenptr) {
    int n;

    n = accept(fd, sa, salenptr);
    if (n < 0) {
        perror("accept error");
        exit(1);
    }
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
Read(int fd, const void *buf, size_t nbytes) {
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
Write(int fd, const void *buf, size_t nbytes) {
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

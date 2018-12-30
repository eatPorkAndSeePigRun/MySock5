#ifndef WRAP_H
#define WRAP_H

#include <sys/socket.h>
#include <sys/epoll.h>

extern int Socket(int family, int type, int protocol);

extern void Bind(int fd, const struct sockaddr *sa, socklen_t salen);

extern void Listen(int fd, int backlog);

extern int Accept(int listenfd, struct sockaddr *sa, socklen_t *salenptr);

extern void Connect(int fd, const struct sockaddr *sa, socklen_t salen);

extern ssize_t Read(int fd, void *buf, size_t nbytes);

extern ssize_t Write(int fd, void *buf, size_t nbytes);

extern int Epoll_create(int size);

extern int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

extern int Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#endif //WRAP_H
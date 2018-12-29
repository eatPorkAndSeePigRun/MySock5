#ifndef WRAP_H
#define WRAP_H

#include <sys/socket.h>

extern int Socket(int family, int type, int protocol = 0);

extern void Bind(int fd, const struct sockaddr *sa, socklen_t salen);

extern void Listen(int fd, int backlog);

extern int Accept(int fd, const struct sockaddr *sa, socklen_t *salenptr);

extern void Connect(int fd, const struct sockaddr *sa, socklen_t salen);

extern ssize_t Read(int fd, const void *buf, size_t nbytes);

extern ssize_t Write(int fd, const void *buf, size_t nbytes);

#endif //WRAP_H

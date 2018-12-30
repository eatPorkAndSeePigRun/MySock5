#ifndef SOCK5_H
#define SOCK5_H

#include <arpa/inet.h>

extern int tcp(const char *ip, uint16_t port);

#define EPOLL_SIZE          5
#define MAX_EPOLL_EVENTS    100
#define BUF_SIZE            10240

extern void run_epoll(int listenfd);

extern void handle_accept(int epollfd, int listenfd);

extern void do_read(int epollfd, int fd, void *buf);

extern void do_write(int epollfd, int fd, void *buf);

#endif  //SOCK5_H

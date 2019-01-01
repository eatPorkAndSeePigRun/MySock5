#ifndef MYEPOLL_H
#define MYEPOLL_H

#include <arpa/inet.h>

extern int tcp(const char *ip, uint16_t port);

#define EPOLL_SIZE          5
#define MAX_EPOLL_EVENTS    100
#define BUF_SIZE            10240

extern void run_epoll(int listenfd);

static void handle_accept(int epollfd, int listenfd);

static void do_read(int epollfd, int fd, void *buf);

static void do_write(int epollfd, int fd, void *buf);

static void run_sock5(int epollfd, int fd, void *buf);

static int client_status[0xff];
static int dest_socket[0xff];
static int client_socket[0xff];

// 表示客户端sock5的请求、连接、转发
#define STATUS_UNINIT   0
#define STATUS_INIT     1
#define STATUS_LICENSE  2
#define STATUS_CONNECT  3

#endif  // MYEPOLL_H

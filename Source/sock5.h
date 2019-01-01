#ifndef SOCK5_H
#define SOCK5_H

#include <arpa/inet.h>

typedef struct {
    char ver;           // 客户端协议版本号，如：0x05, 0x04
    char nmethods;      // 客户端支持认证方式的长度
    char methods[255];  // 客户端支持的认证方式
} client_license_request;

typedef struct {
    char ver;       // 服务端协议版本号
    char method;    // 服务端的认证方式
} server_license_response;

typedef struct {
    char ver;       // 客户端协议版本号
    char cmd;       // 连接方式
    char rsv;       // 保留位
    char type;      // 类型
    char addr[4];   // ip地址
    char port[2];   // 端口号
} client_connect_request;

typedef struct {
    char ver;       // 版本号
    char rep;       // 连接状态
    char rsv;       // 保留位
    char type;      // 类型
    char addr[4];   // ip地址
    char port[2];   // 端口号
} server_connect_response;


#define BOOL    int
#define TRUE    1
#define FALSE   0

extern BOOL sock5_license(void *buf);

extern BOOL sock5_connect(void *buf, int *dest_socketfd);

extern int proxy_socket(struct in_addr *ip, in_port_t *port);

extern void forward(int epollfd, int fd);

#endif  //SOCK5_H

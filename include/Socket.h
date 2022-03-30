/*
 * @Author: Limer
 * @Date: 2022-03-29 13:35:57
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-29 21:27:33
 * @Description: 
 */
#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "InetAddress.h"
class Socket{
    int sockfd;
public:
    Socket();
    Socket(int fd);
    ~Socket();
    void bind(InetAddress* addr);
    void listen();
    int accept(InetAddress* clnt_addr);
    int get_fd();
};
#endif
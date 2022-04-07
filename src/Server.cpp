/*
 * @Author: Limer
 * @Date: 2022-04-03 15:53:26
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-07 13:24:09
 * @Description:
 */
#include "Server.h"
#include <strings.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "util.h"

#define READ_BUFF_SIZE 1024

Server::Server(Eventloop* lp) : _loop(lp) {
    acpt = new Acceptor(lp);
    std::function<void(Socket*)> cb =
        std::bind(&Server::newConnection, this, std::placeholders::_1);
    acpt->setNewConnCallback(cb);
}

Server::~Server() { delete acpt; }

/**
 * @description: 处理读事件
 * @param {int} fd
 * @return {*}
 */
void Server::handleReadEvent(int fd) {
    char buf[READ_BUFF_SIZE];
    while (true) {
        ::bzero(buf, READ_BUFF_SIZE);
        ssize_t read_bytes = ::read(fd, buf, READ_BUFF_SIZE);
        if (read_bytes > 0) {
            ::printf("from: %d, %s\n", fd, buf);
            read_bytes = write(fd, buf, READ_BUFF_SIZE);
            errif(read_bytes == -1, "fail to send message to the client!\n");
        } else if (read_bytes == -1 &&
                   (errno == EAGAIN || errno == EWOULDBLOCK)) {
            ::printf("finish reading\n");
            break;
        } else if (read_bytes == -1 && errno == EINTR) {
            continue;
        } else if (read_bytes == 0) {
            ::printf("client close!\n");
            ::close(fd);
            break;
        }
    }
}

/**
 * @description:
 * 建立新的fd保存新的连接，并且为新的连接设定Channel，以及其对应的回调函数。
 * @param {Socket*} serv_sock
 * @return {*}
 */
void Server::newConnection(Socket* serv_sock) {
    InetAddress* clnt_addr = new InetAddress();
    Socket* clnt_sock = new Socket(serv_sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->get_fd(),
           inet_ntoa(clnt_addr->_addr.sin_addr),
           ntohs(clnt_addr->_addr.sin_port));
    setnonblocking(clnt_sock->get_fd());
    Channel* clntChannel = new Channel(_loop, clnt_sock->get_fd());
    std::function<void()> cb =
        std::bind(&Server::handleReadEvent, this, clnt_sock->get_fd());
    // 添加channel到Epoll_event.
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}

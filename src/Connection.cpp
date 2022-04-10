/*
 * @Author: Limer
 * @Date: 2022-04-08 13:12:07
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-10 17:02:36
 * @Description:
 */
#include "Connection.h"
#include <string.h>
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "unistd.h"

#define READ_BUF_SIZE 1024
Connection::Connection(Eventloop* _loop, Socket* _sock)
    : loop(_loop), sock(_sock) {
    connChl = new Channel(loop, sock->get_fd());
    std::function<void()> cb =
        std::bind(&Connection::echo, this, sock->get_fd());
    connChl->setCallback(cb);
    connChl->enableReading();
}

Connection::~Connection() {
    delete connChl;
    delete sock;
}

void Connection::echo(int sockfd) {
    char buf[READ_BUF_SIZE];
    while (true) {
        bzero(buf, READ_BUF_SIZE);
        ssize_t read_bytes = read(sockfd, buf, READ_BUF_SIZE);
        if (read_bytes > 0) {
            printf("from: %d, %s\n", sockfd, buf);
        } else if (read_bytes == -1 &&
                   (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("finish reading\n");
            break;
        } else if (read_bytes == -1 && errno == EINTR) {
            continue;
        } else if (read_bytes == 0) {
            printf("client close!\n");
            // ! call the callback
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
    deleteConnectionCallback = _cb;
}
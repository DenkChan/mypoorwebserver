/*
 * @Author: Limer
 * @Date: 2022-04-08 13:12:07
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-25 22:03:30
 * @Description:
 */
#include "Connection.h"
#include <string.h>
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "unistd.h"

#define READ_BUF_SIZE 1024
Connection::Connection(Eventloop* _loop, Socket* _sock)
    : loop(_loop), sock(_sock) {
    connChl = new Channel(loop, sock->get_fd());
    readbuf = new Buffer();
    std::function<void()> cb =
        std::bind(&Connection::echo, this, sock->get_fd());
    connChl->setCallback(cb);
    connChl->enableReading();
}

Connection::~Connection() {
    delete connChl;
    delete sock;
    delete readbuf;
}

void Connection::echo(int sockfd) {
    char buf[READ_BUF_SIZE];
    while (true) {
        bzero(buf, READ_BUF_SIZE);
        ssize_t read_bytes = read(sockfd, buf, READ_BUF_SIZE);
        if (read_bytes > 0) {
            readbuf->append(buf, ::strlen(buf));
        } else if (read_bytes == -1 &&
                   (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("message from client:%d, length:%ld, %s\n", sockfd,
                   readbuf->size(), readbuf->c_str());
            printf("finish reading\n");
            write(sockfd, readbuf->c_str(), readbuf->size() + 1);
            readbuf->clear();
            break;
        } else if (read_bytes == -1 && errno == EINTR) {
            continue;
        } else if (read_bytes == 0) {
            printf("client %d close!\n", sockfd);
            // ! call the callback
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
    deleteConnectionCallback = _cb;
}
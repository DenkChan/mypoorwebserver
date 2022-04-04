/*
 * @Author: Limer
 * @Date: 2022-04-03 15:53:26
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-04 14:20:38
 * @Description:
 */
#include "Server.h"
#include <strings.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "util.h"

#define READ_BUFF_SIZE 1024

Server::Server(Eventloop* lp) : _loop(lp) {
  InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
  Socket* serv_sock = new Socket();
  serv_sock->bind(serv_addr);
  serv_sock->listen();
  setnonblocking(serv_sock->get_fd());
  Channel* srvChannel = new Channel(_loop, serv_sock->get_fd());
  std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
  srvChannel->setCallback(cb);
  srvChannel->enableReading();
}

Server::~Server() {}

void Server::handleReadEvent(int fd) {
  char buf[READ_BUFF_SIZE];
  while (true) {
    bzero(buf, READ_BUFF_SIZE);
    ssize_t read_bytes = read(fd, buf, READ_BUFF_SIZE);
    if (read_bytes > 0) {
      printf("from: %d, %s\n", fd, buf);
    } else if (read_bytes == -1 && errno == EAGAIN || errno == EWOULDBLOCK) {
      printf("finish reading\n");
      break;
    } else if (read_bytes == -1 && errno == EINTR) {
      continue;
    } else if (read_bytes == 0) {
      printf("client close!\n");
      close(fd);
      break;
    }
  }
}

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
  clntChannel->setCallback(cb);
  clntChannel->enableReading();
}

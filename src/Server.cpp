/*
 * @Author: Limer
 * @Date: 2022-04-03 15:53:26
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-10 17:01:42
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
#include "Connection.h"
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

void Server::newConnection(Socket* sock) {
    Connection* conn = new Connection(_loop, sock);
    std::function<void(Socket*)> cb =
        std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    conns[sock->get_fd()] = conn;
}

void Server::deleteConnection(Socket* sock) {
    Connection* conn = conns[sock->get_fd()];
    conns.erase(sock->get_fd());
    delete conn;
}

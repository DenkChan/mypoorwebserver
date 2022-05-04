/*
 * @Author: Limer
 * @Date: 2022-04-03 15:53:26
 * @LastEditors: Limer
 * @LastEditTime: 2022-05-03 13:48:49
 * @Description:
 */
#include "Server.h"
#include <strings.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <thread>
#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

#define READ_BUFF_SIZE 1024

Server::Server(Eventloop* lp) : mainReactor(lp), acpt(nullptr) {
    acpt = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb =
        std::bind(&Server::newConnection, this, std::placeholders::_1);
    acpt->setNewConnCallback(cb);

    int size = std::thread::hardware_concurrency();
    thpool = new ThreadPool(size);
    for (int i = 0; i < size; ++i) {
        subReactors.push_back(new Eventloop());
    }

    for (int i = 0; i < size; ++i) {
        std::function<void()> sub_loop =
            // ? why is there a '&';
            std::bind(&Eventloop::loop, subReactors[i]);
        thpool->add(sub_loop);
    }
}

Server::~Server() { delete acpt; }

void Server::newConnection(Socket* sock) {
    Connection* conn = new Connection(mainReactor, sock);
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

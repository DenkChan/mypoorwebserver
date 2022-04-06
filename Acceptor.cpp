/*
 * @Author: Limer
 * @Date: 2022-04-06 21:35:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-06 21:42:31
 * @Description:
 */
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

Acceptor::Acceptor(Eventloop* _loop) : loop(_loop) {
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    setnonblocking(sock->get_fd());
    acptChl = new Channel(_loop, sock->get_fd());
    std::function<void()> cb = std::bind(&Acceptor::acptConn, this);
    acptChl->setCallback(cb);
    acptChl->enableReading();
}

Acceptor::~Acceptor() {
    delete sock;
    delete addr;
    delete acptChl;
}

void Acceptor::acptConn() { newConnCallback(sock); }

void Acceptor::setNewConnCallback(std::function<void(Socket*)> _cb) {
    newConnCallback = _cb;
}
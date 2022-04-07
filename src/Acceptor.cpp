/*
 * @Author: Limer
 * @Date: 2022-04-06 21:35:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-07 13:37:21
 * @Description: 作为一个功能类，负责建立连接并且设定Channel以及后续的回调函数。
 */
#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

// Acceptor的回调函数由对象构造后上层调用设定，其自己会构造一个socket对象并建立与其对应的Channel，
// Channel设定的回调函数为上层设定的回调函数。

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
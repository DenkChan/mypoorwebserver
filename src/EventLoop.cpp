/*
 * @Author: Limer
 * @Date: 2022-04-03 15:35:16
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-24 20:16:45
 * @Description:
 */
#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"

Eventloop::Eventloop() : ep(nullptr), threadpoll(nullptr), quit(false) {
    ep = new Epoll();
    threadpoll = new ThreadPool();
}

Eventloop::~Eventloop() {
    delete ep;
    delete threadpoll;
}

void Eventloop::loop() {
    while (!quit) {
        std::vector<Channel*> chs;
        chs = ep->poll(-1);
        for (auto it : chs) {
            it->handleEvent();
        }
    }
}

void Eventloop::updateChannel(Channel* ch) { ep->updateChannel(ch); }

void Eventloop::addThread(std::function<void()> callback) {
    threadpoll->add(callback);
}
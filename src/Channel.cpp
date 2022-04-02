/*
 * @Author: Limer
 * @Date: 2022-03-31 16:30:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-02 16:23:05
 * @Description:
 */
#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *_ep, int _fd)
    : ep(_ep), fd(_fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {}

void Channel::enableReading() {
  events = EPOLLIN | EPOLLET;
  ep->updateChannel(this);
}

int Channel::getFd() { return fd; }

uint32_t Channel::getEvents() { return events; }
void Channel::setEvents(uint32_t evnts) { events = evnts; }

uint32_t Channel::getRevents() { return revents; }

void Channel::setRevents(uint32_t revnts) { revents = revnts; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setInEpoll(bool inep) { inEpoll = inep; }
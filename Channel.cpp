/*
 * @Author: Limer
 * @Date: 2022-03-31 16:30:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-04 13:56:58
 * @Description:
 */
#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(Eventloop *lp, int _fd)
    : _loop(lp), fd(_fd), events(0), revents(0), inEpoll(false), callback() {}

Channel::~Channel() {}

void Channel::enableReading() {
  events = EPOLLIN | EPOLLET;
  _loop->updateChannel(this);
}

int Channel::getFd() { return fd; }

uint32_t Channel::getEvents() { return events; }
void Channel::setEvents(uint32_t evnts) { events = evnts; }

uint32_t Channel::getRevents() { return revents; }

void Channel::setRevents(uint32_t revnts) { revents = revnts; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setInEpoll(bool inep) { inEpoll = inep; }

void Channel::setCallback(std::function<void()> _cb) { callback = _cb; }

void Channel::handleEvent() { callback(); }
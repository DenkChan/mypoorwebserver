/*
 * @Author: Limer
 * @Date: 2022-03-29 21:30:19
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-01 15:43:29
 * @Description:
 */
#include "Epoll.h"
#include "Channel.h"
#include <sys/epoll.h>

Epoll::Epoll() {
  epfd = epoll_create1(0);
  errif(epfd == -1, "fail to create epollfd.");
  events = new epoll_event[def::MAX_EVENTS];
  bzero(events, sizeof(*events) * def::MAX_EVENTS);
}

Epoll::~Epoll() {
  ::close(epfd);
  delete[] events;
}

void Epoll::addFd(int fd, uint32_t method) {
  setnonblocking(fd);
  epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.events = method;
  ev.data.fd = fd;
  int ret = ::epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
  errif(ret == -1, "fail to add fd to epfd.");
}

std::vector<Channel *> Epoll::poll(int timeout) {
  std::vector<Channel *> activechannels;
  bzero(events, 1024);
  int n_events = ::epoll_wait(epfd, events, def::MAX_EVENTS, timeout);
  for (int i = 0; i < n_events; ++i) {
    Channel *ch = (Channel *)events[i].data.ptr;
    ch->setRevents(events[i].events);
    activechannels.push_back(ch);
  }
  return activechannels;
}

void Epoll::updateChannel(Channel *ch) {
  int fd = ch->getFd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = ch;
  ev.events = ch->getEvents();
  if (!ch->getInEpoll()) {
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    errif(ret == -1, "fail to add fd to epfd!\n");
  } else {
    int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    errif(ret == -1, "fail to change listen events!\n");
  }
}
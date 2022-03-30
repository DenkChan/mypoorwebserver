/*
 * @Author: Limer
 * @Date: 2022-03-29 21:30:19
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-29 21:56:42
 * @Description: 
 */
#include "Epoll.h"

Epoll::Epoll(){
    epfd = epoll_create1(0);
    errif(epfd == -1, "fail to create epollfd.");
}

Epoll::Epoll(int fd):epfd(fd){}

Epoll::~Epoll(){
    ::close(epfd);
}

void Epoll::addFd(int fd, uint32_t method){
    setnonblocking(fd);
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = method;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    errif(ret == -1, "fail to add fd to epfd.");
}

std::vector<epoll_event> Epoll::poll(){
    epoll_event events[1024];
    bzero(events, 1024);
    int n_events = ::epoll_wait(epfd, events, 1024, -1);
    return std::vector<epoll_event>(events, events+n_events);
}
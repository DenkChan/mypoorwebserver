/*
 * @Author: Limer
 * @Date: 2022-03-29 21:12:26
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-29 21:49:14
 * @Description:
 */
#ifndef __EPOLL_H__
#define __EPOLL_H__
#include "util.h"
#include <sys/epoll.h>
#include <sys/types.h>
#include <vector>
#include <unistd.h>
#include <string.h>

class Epoll {
  int epfd;

public:
  Epoll();
  Epoll(int epfd);
  ~Epoll();
  void addFd(int, uint32_t);
  std::vector<epoll_event> poll();
};
#endif

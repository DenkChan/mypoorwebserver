/*
 * @Author: Limer
 * @Date: 2022-04-03 15:11:52
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-04 13:21:58
 * @Description:
 */
#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

class Epoll;
class Channel;

class Eventloop {
 private:
  Epoll *ep;
  bool quit;

 public:
  Eventloop();
  ~Eventloop();
  void loop();
  void updateChannel(Channel *);
};
#endif
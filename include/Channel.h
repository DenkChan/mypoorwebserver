/*
 * @Author: Limer
 * @Date: 2022-03-31 15:56:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-04 13:56:16
 * @Description:
 */
#include <sys/epoll.h>
#include <functional>
#include "EventLoop.h"

class Eventloop;
class Channel {
 private:
  Eventloop *_loop;
  int fd;
  uint32_t events;
  uint32_t revents;
  bool inEpoll;
  std::function<void()> callback;

 public:
  Channel(Eventloop *, int);
  ~Channel();

  void enableReading();
  void handleEvent();

  int getFd();
  uint32_t getEvents();
  void setEvents(uint32_t);
  uint32_t getRevents();
  void setRevents(uint32_t);
  bool getInEpoll();
  void setInEpoll(bool);
  void setCallback(std::function<void()>);
};

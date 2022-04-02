/*
 * @Author: Limer
 * @Date: 2022-03-31 15:56:04
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-01 10:41:31
 * @Description:
 */
#include <sys/epoll.h>
class Epoll;

class Channel {
private:
  Epoll *ep;
  int fd;
  uint32_t events;
  uint32_t revents;
  bool inEpoll;

public:
  Channel(Epoll*, int);
  ~Channel();

  void enableReading();

  int getFd();
  uint32_t getEvents();
  void setEvents(uint32_t);
  uint32_t getRevents();
  void setRevents(uint32_t);
  bool getInEpoll();
  void setInEpoll(bool);
};

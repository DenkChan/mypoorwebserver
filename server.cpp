/*
 * @Author: Limer
 * @Date: 2022-03-29 22:02:54
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-04 14:22:10
 * @Description:
 */
#include "Server.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"

int main() {
  Eventloop *loop = new Eventloop();
  Server *server = new Server(loop);
  loop->loop();
  return 0;
}
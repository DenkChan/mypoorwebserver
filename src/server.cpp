/*
 * @Author: Limer
 * @Date: 2022-03-29 22:02:54
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-02 16:14:24
 * @Description:
 */
#include <errno.h>
#include <stdio.h>
#include "Channel.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"

int main() {
  InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
  Socket *serv_sock = new Socket();
  Epoll *ep_fd = new Epoll();

  serv_sock->bind(serv_addr);
  serv_sock->listen();
  setnonblocking(serv_sock->get_fd());
  Channel *serv_chl = new Channel(ep_fd, serv_sock->get_fd());
  serv_chl->enableReading();
  for (;;) {
    std::vector<Channel *> activeChannels = ep_fd->poll(-1);
    for (int i = 0; i < activeChannels.size(); ++i) {
      int chfd = activeChannels[i]->getFd();
      if (activeChannels[i]->getFd() == serv_sock->get_fd()) {
        // ! delete
        InetAddress *clnt_addr = new InetAddress();
        // ! memory leak
        Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
        setnonblocking(clnt_sock->get_fd());
        ::printf("new connection,fd:%d , IP: %s, port: %d.\n",
                 clnt_sock->get_fd(), ::inet_ntoa(clnt_addr->_addr.sin_addr),
                 ::ntohs(clnt_addr->_addr.sin_port));
        Channel *clntChannel = new Channel(ep_fd, clnt_sock->get_fd());
        clntChannel->enableReading();
      } else if (activeChannels[i]->getRevents() & EPOLLIN) {
        char buf[1024];
        while (true) {
          bzero(buf, 1024);
          ssize_t read_bytes = read(chfd, buf, 1024);
          if (read_bytes > 0) {
            printf("socket:%d,%s\n", chfd, buf);
            write(chfd, buf, read_bytes);
          } else if (read_bytes == -1 && errno == EINTR) {
            continue;
          } else if (read_bytes == -1 && errno == EAGAIN) {
            printf("finish reading\n");
            break;
          } else if (read_bytes == 0) {
            printf("client socket isn't connection!\n");
            //  closing fd automaticly lead to remove from epoll tree.
            close(chfd);
            break;
          }
        }
      } else {
        ::printf("some else happen!\n");
      }
    }
  }
  delete serv_addr;
  delete serv_sock;
  delete ep_fd;
}
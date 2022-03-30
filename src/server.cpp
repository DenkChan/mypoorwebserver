/*
 * @Author: Limer
 * @Date: 2022-03-29 22:02:54
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-30 13:30:12
 * @Description:
 */
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include <iostream>
#include <stdio.h>

int main() {
  InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
  Socket *serv_sock = new Socket();
  Epoll *ep_sock = new Epoll();
  serv_sock->bind(serv_addr);
  serv_sock->listen();
  ep_sock->addFd(serv_sock->get_fd(), EPOLLIN | EPOLLET);
  for (;;) {
    std::vector<epoll_event> evnts = ep_sock->poll();
    for (int i = 0; i < evnts.size(); ++i) {
      if (evnts[i].data.fd == serv_sock->get_fd()) {
        // ! delete
        InetAddress *clnt_addr = new InetAddress();
        // ! memory leak
        Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
        ep_sock->addFd(clnt_sock->get_fd(), EPOLLIN | EPOLLET);
        std::cout << "new connection, IP:"
                  << ::inet_ntoa(clnt_addr->_addr.sin_addr) << ",fd"
                  << clnt_sock->get_fd() << std::endl;
      } else if (evnts[i].events & EPOLLIN) {
        char buf[1024];
        while (true) {
          bzero(buf, 1024);
          ssize_t read_bytes = read(evnts[i].data.fd, buf, 1024);
          if (read_bytes > 0) {
            printf("socket:%d,%s\n", evnts[i].data.fd, buf);
            write(evnts[i].data.fd, buf, read_bytes);
          } else if (read_bytes == -1 && errno == EINTR) {
            continue;
          } else if (read_bytes == -1 && errno == EAGAIN) {
            printf("finish reading\n");
            break;
          } else if (read_bytes == 0) {
            printf("client socket isn't connection!\n");
            //  closing fd automaticly lead to remove from epoll tree.
            close(evnts[i].data.fd);
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
  delete ep_sock;
}
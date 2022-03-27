/*
 * @Author: Limer
 * @Date: 2022-03-27 16:08:41
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-27 21:00:27
 */
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

void errif(bool condition, const char *errmsg) {
  if (condition) {
    perror(errmsg);
    exit(1);
  }
}
int setnonblocking(int fd) {
  int old_flag = fcntl(fd, F_GETFD);
  fcntl(fd, F_SETFD, old_flag | O_NONBLOCK);
  return old_flag;
}

void epolladdfd(int epfd, int fd) {
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.fd = fd;
  ev.events = EPOLLIN;
  setnonblocking(fd);
  epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int main(int argc, char **argv) {
  int ret = -1;
  char buf[1024] = {
      '\0',
  };
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  errif(sockfd == -1, "fail to create socket fd!");

  int epfd = epoll_create1(0);

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8888);

  ret = bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));
  errif(ret == -1, "fail to bind socket fd and sockaddr!");

  ret = listen(sockfd, SOMAXCONN);
  errif(ret == -1, "fail to listen socket fd!");

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_len = sizeof(clnt_addr);
  epolladdfd(epfd, sockfd);
  struct epoll_event events[1024];
  bzero(events,sizeof(events));
  for(;;) {
    int nev = epoll_wait(epfd, events, 1024, -1);
	errif(ret == -1, "fail to wait epoll events!\n");
    for (int i = 0; i < nev; ++i) {
      if (events[i].data.fd == sockfd) {
        bzero(&clnt_addr, clnt_addr_len);
        int clnt_sockfd =
            accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
        epolladdfd(epfd, clnt_sockfd);
        printf("new client fd %d! IP: %s, Port: %d\n", clnt_sockfd,
               inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
      } else if (events[i].events & EPOLLIN) {
        int clnt_sockfd = events[i].data.fd;
        bzero(buf, sizeof(buf));
        ret = recv(clnt_sockfd, buf, 1024, 0);
        if (ret > 0) {
          printf("sockfd:%d, %s\n", clnt_sockfd, buf);
        } else if (ret < 0) {
          errif(true, "socket receive message error!");
          break;
        }else{
		  printf("EOF!\n");
		  close(clnt_sockfd);
		}
        ret = send(clnt_sockfd, buf, sizeof(buf), 0);
        errif(ret == -1, "fail to send message!");
      } else {
        printf("event number: %d\n", ret);
        printf("some else event happen!\n");
      }
    }
  }
  return 0;
}

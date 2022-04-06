/*
 * @Author: Limer
 * @Date: 2022-04-03 15:49:40
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-06 21:52:08
 * @Description:
 */
#ifndef __SERVER_H__
#define __SERVER_H__
class Eventloop;
class Socket;
class Acceptor;

class Server {
   private:
    Eventloop* _loop;
    Acceptor* acpt;

   public:
    Server(Eventloop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket* serv_sock);
};
#endif
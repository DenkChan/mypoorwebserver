/*
 * @Author: Limer
 * @Date: 2022-04-03 15:49:40
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-10 16:46:17
 * @Description:
 */
#ifndef __SERVER_H__
#define __SERVER_H__
#include <unordered_map>
class Eventloop;
class Socket;
class Acceptor;
class Connection;

class Server {
   private:
    Eventloop* _loop;
    Acceptor* acpt;
    std::unordered_map<int, Connection*> conns;

   public:
    Server(Eventloop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket* serv_sock);
    void deleteConnection(Socket* sock);
};
#endif
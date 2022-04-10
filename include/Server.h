/*
 * @Author: Limer
 * @Date: 2022-04-03 15:49:40
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-10 18:28:29
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

    void newConnection(Socket*);
    void deleteConnection(Socket*);
};
#endif
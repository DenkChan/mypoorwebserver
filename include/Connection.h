/*
 * @Author: Limer
 * @Date: 2022-04-07 21:49:36
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-10 16:27:40
 * @Description:
 */
#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <functional>
class Eventloop;
class InetAddress;
class Socket;
class Channel;
class Connection {
   private:
    Eventloop* loop;
    Socket* sock;
    Channel* connChl;
    std::function<void(Socket*)> deleteConnectionCallback;

   public:
    Connection(Eventloop*, Socket*);
    ~Connection();
    void echo(int);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};
#endif
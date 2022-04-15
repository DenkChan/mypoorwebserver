/*
 * @Author: Limer
 * @Date: 2022-03-27 16:27:28
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-15 19:56:28
 * @Description:
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Buffer.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"

int main(int argc, char** argv) {
    int ret = -1;
    Socket* sock = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
    sock->connect(serv_addr);

    Buffer* writebuf = new Buffer();
    // Buffer* readbuf = new Buffer();
    while (1) {
        writebuf->getline();
        ssize_t writebytes =
            write(sock->get_fd(), writebuf->c_str(), writebuf->size());
        if (writebytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
    }
    delete sock;
    delete serv_addr;
    delete writebuf;
    return 0;
}

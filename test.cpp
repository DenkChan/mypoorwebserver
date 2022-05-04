/*
 * @Author: Limer
 * @Date: 2022-04-25 20:59:26
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-25 22:08:02
 * @Description:A stress test program.
 */
#include <string.h>
#include <unistd.h>
#include <functional>
#include <iostream>
#include "Buffer.h"
#include "InetAddress.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

void oneClient(int msgs, int wait) {
    Socket* sock = new Socket();
    InetAddress* addr = new InetAddress("127.0.0.1", 8888);
    sock->connect(addr);

    int sockfd = sock->get_fd();
    // setnonblocking(sockfd);
    Buffer* sendBuffer = new Buffer();
    Buffer* readBuffer = new Buffer();

    sleep(wait);

    int count = 0;
    while (count < msgs) {
        sendBuffer->setBuf("I'm client!");
        ssize_t write_bytes =
            write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024];  //这个buf大小无所谓
        while (true) {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0) {
                readBuffer->append(buf, read_bytes);
                already_read += read_bytes;
            } else if (read_bytes == 0) {  // EOF
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if (already_read >= sendBuffer->size()) {
                printf("count: %d, message from server: %s\n", count++,
                       readBuffer->c_str());
                break;
            }
        }
        readBuffer->clear();
    }
}

int main(int argc, char* argv[]) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char* optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 't':
                threads = atoi(optarg);
                break;
            case 'm':
                msgs = atoi(optarg);
                break;
            case 'w':
                wait = atoi(optarg);
                break;
            case '?':
                printf("error optopt:%c \n", optopt);
                printf("error optopt:%d \n", opterr);
                break;
            default:
                break;
        }
    }
    ThreadPool* pool = new ThreadPool(threads);
    std::function<void()> func = std::bind(oneClient, msgs, wait);
    for (int i = 0; i < threads; ++i) pool->add(func);
    delete pool;
    return 0;
}

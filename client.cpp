/*
 * @Author: Limer
 * @Date: 2022-03-27 16:27:28
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-10 18:31:56
 * @Description:
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void errif(bool condition, const char* msg) {
    if (condition == true) {
        perror(msg);
        exit(1);
    }
}

int main(int argc, char** argv) {
    int ret = -1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buf[1024];
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    ret = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    errif(ret == -1, "fail to connect server!");
    while (1) {
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        ret = send(sockfd, buf, sizeof(buf), 0);
        if (ret == -1) {
            perror(strerror(errno));
        }
    }
    close(sockfd);
    return 0;
}

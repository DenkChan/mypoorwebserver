#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>

int main(int argc, char** argv){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8888);

	connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
	close(sockfd);
	return 0;
}


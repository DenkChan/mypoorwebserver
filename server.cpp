#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

void errif(bool condition, const char* errmsg){
	if(condition){
		perror(errmsg);
		exit(1);	
	}
}

int main(int argc, char** argv){
	int ret = -1;
	char buf[1024] = {'\0',};
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errif(sockfd == -1, "fail to create socket fd!");

	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8888);

	ret = bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
	errif(ret == -1, "fail to bind socket fd and sockaddr!");

	ret = listen(sockfd, SOMAXCONN);
	errif(ret == -1, "fail to listen socket fd!");

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof(clnt_addr);
	bzero(&clnt_addr, clnt_addr_len);

	int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
	errif(clnt_sockfd == -1, "fail to listen socket fd!");

	printf("new client fd %d! IP: %s, Port: %d\n",clnt_sockfd,
												  inet_ntoa(clnt_addr.sin_addr), 
												  ntohs(clnt_addr.sin_port));
	while(1){
		bzero(buf, sizeof(buf));
		ret = recv(clnt_sockfd, buf, 1024, 0);		
		if(ret > 0){
			printf("%s\n", buf);
		}else if(ret < 0){
			errif(true, "socket receive message error!");
			break;
		}
	    ret = send(clnt_sockfd, buf, ret, 0); 	
		errif(ret == -1, "fail to send message!");
	}
	close(clnt_sockfd);
	close(sockfd);
	return 0;
}


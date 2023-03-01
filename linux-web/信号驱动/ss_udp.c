#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>

#define PRINT_ERR(check, value, note) \
		if (value == check) \
        { \
        	perror(note); \
			exit(EXIT_FAILURE); \
		}

int g_sockfd = 0;

void init()
{
	g_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	PRINT_ERR(g_sockfd, -1, "socket");

	fcntl(g_sockfd, F_SETOWN, getpid());
	int flag = fcntl(g_sockfd, F_GETFL, 0);
	flag |= O_ASYNC|O_NONBLOCK;
	fcntl(g_sockfd, F_SETFL, flag);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(saddr.sin_zero, 8);

	int val = 1;
	int ret = setsockopt(g_sockfd, SOL_SOCKET, SO_REUSEADDR
			             , &val, sizeof(int));	
	PRINT_ERR(ret, -1, "setsockopt");

	ret = bind(g_sockfd, (struct sockaddr*)&saddr
			   , sizeof(saddr));
	PRINT_ERR(ret, -1, "bind");
	printf("init sockfd=%d\n", g_sockfd);
}

void handleClient(int sig)
{
	printf("sigio= %d, sig=%d\n", SIGIO, sig);
	if (SIGIO == sig)
	{
		struct sockaddr_in caddr;
		unsigned int uiAddrLen = sizeof(caddr); 
		char caMsg[128] = {'\0'};
		ssize_t ret = -1;
		printf("sockfd=%d\n", g_sockfd);
		ret = recvfrom(g_sockfd, caMsg, sizeof(caMsg), 0
				       , (struct sockaddr*)&caddr, &uiAddrLen);
		if (ret > 0)
		{
			printf("收到客户端信息:%s\n", caMsg);
		}
		if (0 == ret)
		{
			printf("peer shut down\n");
		}
		if (ret < 0)
		{
			perror("recvfrom");
		}
	}
}

int main(void)
{
	init();

	signal(SIGIO, handleClient);
	while (1)
	{
		printf("主程序挂起中...\n");
		pause();
	}	

	return 0;
}

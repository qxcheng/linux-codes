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

int init()
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	PRINT_ERR(sockfd, -1, "socket");

	return sockfd;
}

int main(void)
{
	int sockfd = init();

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(saddr.sin_zero, 8);
	
	char caMsg[128] = {'\0'};
	while (1)
	{
		memset(caMsg, '\0', sizeof(caMsg));
		printf("请输入要发送的信息:\n");
		scanf("%s", caMsg);
		if (0 == strcmp(caMsg, "quit"))
		{
			break;
		}
		sendto(sockfd, caMsg, strlen(caMsg), 0
			   , (struct sockaddr*)&saddr, sizeof(saddr));
	}	

	return 0;
}

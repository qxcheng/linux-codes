#include <stdio.h>
#include <sys/types.h>   //socket
#include <sys/socket.h>
#include <errno.h>       //errno
#include <string.h>      //strerror
#include <stdlib.h>      //exit
#include <netinet/in.h>  //inet_addr
#include <arpa/inet.h>
#include <unistd.h>      //close
#include <pthread.h>

int main(void)
{
	//SOCK_DGRAM:可靠传输-->udp
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		printf("errno=%d, error=%s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;  	
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(8888);
	bzero(saddr.sin_zero, 8);

	char caMsg[1024] = {'\0'};
	int iLen = sizeof(saddr);
	
	while (1)
	{
		memset(caMsg, 0, sizeof(caMsg));
		printf("please input msg:\n");
		scanf("%s", caMsg);
		sendto(sockfd, caMsg, strlen(caMsg), 0 
			   , (struct sockaddr *)&saddr
			   , sizeof(saddr));
		if (0 == strcmp("quit", caMsg))
		{
			sleep(2);
			break;
		}

		memset(caMsg, 0, sizeof(caMsg));
		recvfrom(sockfd, caMsg, strlen(caMsg), 0 
			     , (struct sockaddr *)&saddr
			     , &iLen);
		printf("recv msg:%s\n", caMsg);
	}

	close(sockfd);	

	return 0;
}

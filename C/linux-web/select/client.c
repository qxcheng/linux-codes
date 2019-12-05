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
	//建立TCP连接
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		printf("errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET; 
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(8888);
	bzero(saddr.sin_zero, 8);
	
	int ret = connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (-1 == ret)
	{
		printf("connect::errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("connect to server ok\n");

	fd_set rset;
	char caMsg[128] = {'\0'};
	
	while (1)
	{
		FD_ZERO(&rset); //清空集合
		
		FD_SET(STDIN_FILENO, &rset); //将要监控的fd添加到集合中
		FD_SET(sockfd, &rset);
		
		ret = select(sockfd+1, &rset, NULL, NULL, NULL);
		//成功返回将要进行读操作的文件描述符的个数
		//失败返回-1
		if (-1 == ret)
		{
			perror("select");
			break;
		}
		if (FD_ISSET(sockfd, &rset))
		{
			memset(caMsg, 0, sizeof(caMsg));
			if (0 == read(sockfd, caMsg, sizeof(caMsg)))
			{
				break;
			}
			printf("recv msg:%s\n", caMsg);
		}
		if (FD_ISSET(STDIN_FILENO, &rset)) 
		{
			memset(caMsg, 0, sizeof(caMsg));
			scanf("%s", caMsg);
			write(sockfd, caMsg, strlen(caMsg));	
		}
	}

	close(sockfd);	
	
	return 0;
}

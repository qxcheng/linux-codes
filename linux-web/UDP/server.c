#include <stdio.h>
#include <sys/types.h>   //socket
#include <sys/socket.h>
#include <errno.h>       //errno
#include <string.h>      //strerror
#include <stdlib.h>      //exit
#include <netinet/in.h>  //inet_addr
#include <arpa/inet.h>
#include <unistd.h>      //close
#include <pthread.h>     //pthread_create
#include <sys/types.h>   //open
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	//SOCK_DGRAM:可靠传输-->udp
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		printf("errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in saddr, caddr;	
	saddr.sin_family = AF_INET; //ipv4	
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //将点分十进制字符串表示的ip转换成整数表示
	saddr.sin_port = htons(8888); //将本地数据转换成网络字节序表示
	bzero(saddr.sin_zero, 8); //将指定的空间置为0
	
	//设置要监听的ip和port
	int ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (-1 == ret)
	{
		printf("bind::errno=%d, error=%s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	//用于保存连接过来的客户端的ip,port等信息
	int iLen = sizeof(caddr);
	char caMsg[1024] = {'\0'};
	while (1)
	{
		memset(caMsg, 0, sizeof(caMsg));
		recvfrom(sockfd, caMsg, sizeof(caMsg), 0, (struct sockaddr*)&caddr, &iLen);
		printf("recv msg:%s\n", caMsg);

		strcat(caMsg, " ^_^ !");
		sendto(sockfd, caMsg, strlen(caMsg), 0, (struct sockaddr*)&caddr, iLen);
	}

	return 0;
}

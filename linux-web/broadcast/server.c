#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define RET_OK 0
#define RET_ERR -1
#define LISTEN_QUEUE_NUM 5
#define BUFFER_SIZE 256
#define ECHO_PORT 2029

int main(int argc, char *argv[])
{
	int sockfd, ret = RET_OK, opt = 1;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in servaddr, recvaddr;
	struct hostent *server;
		
	if (argc < 2) {
		fprintf(stderr,"usage %s hostname\n", argv[0]);
		return RET_ERR;
	}
	if((server = gethostbyname(argv[1])) == NULL)
	{
		herror("gethostbyname. ");
		return RET_ERR;
	}
	
	//UDP套接字
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("ERROR opening socket");
		return RET_ERR;
	}
	
	//允许广播
	if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)))
	{
		perror("ERROR setsockopt");
		goto failed;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = *(uint32_t *)server->h_addr;
	servaddr.sin_port = htons((uint16_t)ECHO_PORT);  //2029
	
	while(1)
	{
		printf("Enter the message : ");
		if(fgets(buffer, sizeof(buffer) - 1, stdin) == NULL)
		{
			break;
		}
		
		//直接向广播地址发消息
		if((ret = sendto(sockfd, buffer ,strlen(buffer), 0, (struct
		sockaddr *)&servaddr, sizeof(servaddr))) < 0)
		{
			perror("ERROR writing to socket");
			break;
		}
		
		uint32_t len = sizeof(recvaddr);
		if((ret = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct
		sockaddr *)&recvaddr, &len)) < 0)
		{
			perror("ERROR reading from socket");
			break;
		}
		printf("Relay from: %s %u\n", inet_ntoa(recvaddr.sin_addr),
		htons(recvaddr.sin_port));buffer[ret] = 0;
		printf("Server echo message: %s\n",buffer);
	}
		
failed:
	close(sockfd);
	
	return ret < 0 ? RET_ERR : RET_OK;
	
}
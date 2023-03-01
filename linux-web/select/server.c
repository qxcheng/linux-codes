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
#include <sys/time.h>
#include "list.h"

UsrList *g_pList = NULL;
int g_iMaxFD = 0;

//初始化fd_set集合
void initSet(fd_set *set)
{
	UsrNode *node = g_pList->pFirstNode;
	while (NULL != node)
	{
		FD_SET(node->sockfd, set);
		if (g_iMaxFD < node->sockfd)
		{
			g_iMaxFD = node->sockfd;
		}
		node = node->pNext;
	}
}

//处理客户端消连接
void acceptClient(int sockfd)
{
	struct sockaddr_in caddr;
	int iLen = sizeof(caddr);
	int csockfd = -1;
	printf("accept client ...\n");
	csockfd = accept(sockfd, (struct sockaddr*)&caddr, &iLen);
	if (-1 == csockfd)
	{
		perror("accept");
		return;
	}

	//显示客户端的ip,port
	printf("csockfd=%d, c_ip=%s, c_port=%d\n"
		   , csockfd, inet_ntoa(caddr.sin_addr)
		   , ntohs(caddr.sin_port));
	UsrNode *node = mkUsrNode();
	node->sockfd = csockfd;
	insertUsrList(g_pList, node);
}

//处理客户端消息
void handleClientMsg(fd_set *set)
{
	UsrNode *node = g_pList->pFirstNode;
	char caMsg[128] = {'\0'};
	while (NULL != node)
	{
		if (FD_ISSET(node->sockfd, set))
		{
			memset(caMsg, 0, sizeof(caMsg));
			read(node->sockfd, caMsg, sizeof(caMsg));
			strcat(caMsg, " -_-....\n");
			write(node->sockfd, caMsg, strlen(caMsg));
		}
		node = node->pNext;
	}
}

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
	int ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (-1 == ret)
	{
		printf("bind::errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
		
	ret = listen(sockfd, 25);	
	if (-1 == ret)
	{
		printf("listen::errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	g_pList = mkUsrList(); //初始化链表
	fd_set rset;
	
	while (1)
	{
		FD_ZERO(&rset); 
		FD_SET(sockfd, &rset); //监听套接字加入集合
		g_iMaxFD = sockfd;     //文件描述符最大数量
		initSet(&rset);	
		
		ret = select(g_iMaxFD+1, &rset, NULL, NULL, NULL);
		if (-1 == ret)
		{
			perror("select");
			break;
		}
		
		//表示有客户端连接过来
		if (FD_ISSET(sockfd, &rset))
		{
			acceptClient(sockfd);	
		}
		
		handleClientMsg(&rset);  //处理客户端消息
	}	
	
	return 0;
}

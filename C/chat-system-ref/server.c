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
#include "protocol.h"

#include "fileope.h"

UsrList *g_pUsrList = NULL;

void handelClientRegistRequest(PDU *pdu, int sockfd)
{
	UsrNode *node = mkUsrNode();
	node->uiId = g_pUsrList->uiNewId;
	g_pUsrList->uiNewId++;

	strcpy(node->caPwd, pdu->caMsg);
	insertUsrList(g_pUsrList, node);
	saveUsrInfoToFile(g_pUsrList);

	PDU *respdu = mkPDU(0);
	respdu->uiTo = node->uiId;
	respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;
	sendMsg(respdu, sockfd);
	free(respdu);	
	respdu = NULL;
}
			
void handelClientLoginRequest(PDU *pdu, int sockfd)
{
	if (NULL == pdu)
	{
		return;
	}
	UsrNode *node = findUsrById(g_pUsrList, pdu->uiFrom);
	PDU *respdu = NULL;
	if (NULL == node)
	{
		respdu = mkPDU(strlen("id error")+1);
		strcpy(respdu->caMsg, "id error");
	}
	else
	{
		if (0 == strcmp(node->caPwd, pdu->caMsg))
		{
			respdu = mkPDU(strlen("login ok")+1);
			strcpy(respdu->caMsg, "login ok");
			respdu->uiTo = pdu->uiFrom;
			node->sockfd = sockfd;
		}
		else
		{
			respdu = mkPDU(strlen("pwd error")+1);
			strcpy(respdu->caMsg, "pwd error");
		}
	}
	respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
	sendMsg(respdu, sockfd);
	free(respdu);	
	respdu = NULL;
}
			
void handleClientGetFriendListRequest(int sockfd)
{
	PDU *pdu = mkPDU(g_pUsrList->uiLen*sizeof(uint));
	pdu->uiMsgType = ENUM_MSG_TYPE_GET_FRIEND_LIST_RESPOND;
	UsrNode *node = g_pUsrList->pFirstNode;
	int i = 0;
	while (NULL != node)
	{
		memcpy(pdu->caMsg+i*sizeof(uint), &(node->uiId), sizeof(uint));
		node = node->pNext;
		i++;
	}
	sendMsg(pdu, sockfd);
	free(pdu);
	pdu = NULL;
}


void handleClientPrivateChatRequest(PDU *pdu, int sockfd)
{
	if (NULL == pdu)
	{
		return;
	}

	UsrNode *node = findUsrById(g_pUsrList, pdu->uiTo);
	PDU *respdu = NULL;
	if (NULL != node)
	{
		if (node->sockfd > 0)
		{
			sendMsg(pdu, node->sockfd);
		}
		else
		{
			respdu = mkPDU(strlen("per offline")+1);
			strcpy(respdu->caMsg, "per offline");
		}	
	}
	else
	{
		respdu = mkPDU(strlen("per error")+1);
		strcpy(respdu->caMsg, "per error");
	}
	if (NULL != respdu)
	{
		respdu->uiMsgType = ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND;
		sendMsg(respdu, sockfd);
		free(respdu);
		respdu = NULL;
	}
}
			
void handleClientGroupChatRequest(PDU *pdu, int sockfd)
{
	if (NULL == pdu)
	{
		return;
	}
	UsrNode *node = g_pUsrList->pFirstNode;
	while (NULL != node)
	{
		if (node->sockfd > 0)
		{
			sendMsg(pdu, node->sockfd);
		}
		node = node->pNext;
	}
}

void *handleClient(void *arg)
{
	int sockfd = (int)arg;
	PDU *pdu = NULL;
	while (1)
	{
		pdu = recvMsg(sockfd);
		if (NULL == pdu)
		{
			break;
		}
		switch (pdu->uiMsgType)
		{
		case ENUM_MSG_TYPE_REGIST_REQUEST:
			handelClientRegistRequest(pdu, sockfd);
			break;
		case ENUM_MSG_TYPE_LOGIN_REQUEST:
			handelClientLoginRequest(pdu, sockfd);
			break;
		case ENUM_MSG_TYPE_GET_FRIEND_LIST_REQUEST:
			handleClientGetFriendListRequest(sockfd);
			break;
		case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
			handleClientPrivateChatRequest(pdu, sockfd);
			break;
		case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
			handleClientGroupChatRequest(pdu, sockfd);
			break;
		}
		free(pdu);
		pdu = NULL;
	}
	return NULL;
}


int main(void)
{
	g_pUsrList = mkUsrList();
	getUsrInfoFromFile(g_pUsrList);	

	//AF_INET:ipv4
	//SOCK_STREAM:可靠传输-->tcp
	//0:表示使用指定传输方式的默认协议
	//用于监听客户端的连接
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		//errno:系统全局的变量，
		//记录最新发生的系统调用错误编号
		//strerror:获得错误编号对应的错误信息
		printf("errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;  //ipv4
	//inet_addr:将点分十进制字符串表示的ip转换成整数表示
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//将本地数据转换成网络字节序表示
	saddr.sin_port = htons(8888);
	//将指定的空间置为0
	bzero(saddr.sin_zero, 8);
	//设置要监听的ip和port
	int ret = bind(sockfd, (struct sockaddr *)&saddr
				   , sizeof(saddr));
	if (-1 == ret)
	{
		printf("bind::errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	//将sockfd设置为处于监听状态，并设置连接的缓存队列
	ret = listen(sockfd, 25);	
	if (-1 == ret)
	{
		printf("listen::errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	//caddr:用于保存连接过来的客户端的ip,port等信息
	struct sockaddr_in caddr;
	int iLen = sizeof(caddr);
	int csockfd = -1;
	pthread_t thr;
	while (1)
	{
		printf("accept client ...\n");
		//accept:阻塞等待接收客户端的连接
		//当成功接收到客户端的连接时
		//会自动将客户端的ip,port等信息填入caddr中
		//并会产生一个新的socket,
		//用服务器和客户端进行数据交互
		csockfd = accept(sockfd
						 , (struct sockaddr*)&caddr
						 , &iLen);
		if (-1 == csockfd)
		{
			//直接将出错信息显示出来
			//accept:error
			perror("accept");
			continue;
		}

		//显示客户端的ip,port
		printf("csockfd=%d, c_ip=%s, c_port=%d\n"
			   , csockfd, inet_ntoa(caddr.sin_addr)
			   , ntohs(caddr.sin_port));
		//每来一个客户端，都产生一个线程专门处理	
		pthread_create(&thr, NULL, handleClient
					   , (void *)csockfd);
	}


	return 0;
}

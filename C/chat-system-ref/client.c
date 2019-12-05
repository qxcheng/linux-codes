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
#include "protocol.h"

int g_uiId = 0;

int getIntFromSTDIN()
{
	char caBuf[4096] = {'\0'};
	read(STDIN_FILENO, caBuf, 4096);
	//反向查找caBuf中是否存在'\n'
	//若存在，则表示将输入的数据都读取完了
	//返回'\n'在caBuf中的地址
	char *p = strrchr(caBuf, '\n');//从后往前找
	//char *p = strchr(caBuf, '\n');//从前往后找
	if (NULL != p)
	{
		*p = '\0';
	}
	else  //若在数组中没有找到'\n',则返回NULL 
	{
		caBuf[4095] = '\0';	
		//循环将剩余数据读掉
		while ('\n' != getchar())
		{}
	}

	int iNum = -1;
	//从字符串中按照指定的格式读取数据
	sscanf(caBuf, "%d", &iNum);
	return iNum;
}

int loginOrRegist()
{
	printf("  ^_^\n");
	printf("1,login\n");
	printf("2,regist\n");
	printf("0,exit\n");
	printf("please input choice:\n");
	int iNum = getIntFromSTDIN();
	return iNum;	
}

void getStrFromSTDIN(char *buf, int size)
{
	read(STDIN_FILENO, buf, size);
	char *p = strrchr(buf, '\n');
	if (NULL != p)
	{
		*p = '\0';
	}
	else
	{
		buf[size-1] = '\0';
		while ('\n' != getchar())
		{}
	}
}

void regist(int sockfd)
{
	char caPwd[32] = {'\0'};
	printf("please input regist pwd:\n");
	getStrFromSTDIN(caPwd, 32);
	PDU *pdu = mkPDU(strlen(caPwd)+1);
	strcpy(pdu->caMsg, caPwd);
	pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
	sendMsg(pdu, sockfd);
	free(pdu);
	pdu = NULL;

	pdu = recvMsg(sockfd);
	if (NULL == pdu)
	{
		printf("disconnected...\n");
		return;
	}
	if (ENUM_MSG_TYPE_REGIST_RESPOND == pdu->uiMsgType)
	{
		printf("get id = %u\n", pdu->uiTo);
	}
	free(pdu);
	pdu = NULL;
}

int login(int sockfd)
{
	PDU *pdu = mkPDU(32);
	printf("please input login id:\n");
	pdu->uiFrom = getIntFromSTDIN();
	printf("please input login pwd:\n");
	getStrFromSTDIN(pdu->caMsg, 32);
	pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
	sendMsg(pdu, sockfd);
	free(pdu);
	pdu = NULL;
	
	pdu = recvMsg(sockfd);
	if (NULL == pdu)
	{
		printf("disconnected...\n");
		return 0;
	}
	int sign = 0;
	if (ENUM_MSG_TYPE_LOGIN_RESPOND == pdu->uiMsgType)
	{
		printf("login respond:%s\n", pdu->caMsg);
		if (0 == strcmp("login ok", pdu->caMsg))
		{
			g_uiId = pdu->uiTo;
			sign = 1;
		}
	}
	free(pdu);
	pdu = NULL;
	return sign;
}

int chatFace()
{
	printf("  ^_^\n");
	printf("1,private chat\n");
	printf("2,group chat\n");
	printf("3,get friend list\n");
	printf("0,quit\n");
	printf("please input choice:\n");
	int iNum = getIntFromSTDIN();
	return iNum;
}

void getFriendList(int sockfd)
{
	PDU *pdu = mkPDU(0);
	pdu->uiMsgType = ENUM_MSG_TYPE_GET_FRIEND_LIST_REQUEST;
	sendMsg(pdu, sockfd);
	free(pdu);
	pdu = NULL;	
}
			
void privateChat(int sockfd)
{
	uint uiPerId = 0;
	printf("please input per id:\n");
	uiPerId = getIntFromSTDIN();
	char caMsg[4096] = {'\0'};
	printf("please input msg:\n");
	getStrFromSTDIN(caMsg, 4096);
	PDU *pdu = mkPDU(strlen(caMsg)+1);
	pdu->uiFrom = g_uiId;
	pdu->uiTo = uiPerId;
	strcpy(pdu->caMsg, caMsg);
	pdu->uiMsgType = ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
	sendMsg(pdu, sockfd);
	free(pdu);
	pdu = NULL;
}
			
void groupChat(int sockfd)
{
	char caMsg[4096] = {'\0'};
	printf("please input msg:\n");
	getStrFromSTDIN(caMsg, 4096);
	PDU *pdu = mkPDU(strlen(caMsg)+1);
	pdu->uiFrom = g_uiId;
	strcpy(pdu->caMsg, caMsg);
	pdu->uiMsgType = ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
	sendMsg(pdu, sockfd);
	free(pdu);
	pdu = NULL;
}

void chat(int sockfd)
{
	int iNum = 0;
	while (1)
	{
		iNum = chatFace();
		switch (iNum)
		{
		case 1:
			privateChat(sockfd);
			break;
		case 2:
			groupChat(sockfd);
			break;
		case 3:
			getFriendList(sockfd);
			break;
		case 0:
			break;
		default:
			break;
		}
	}
}
	

void handleServerGetFriendListRespond(PDU *pdu)
{
	uint uiId = 0;
	uint uiNum = pdu->uiMsgLen/sizeof(uint);
	uint i = 0;
	printf("get friend list:\n");
	for (; i<uiNum; i++)
	{
		memcpy(&uiId, pdu->caMsg+i*sizeof(uint), sizeof(uint));
		printf("friend_id:%u\n", uiId);
	}
}

void *handleServer(void *arg)
{
	int sockfd = (int)arg;
	PDU *pdu = NULL;
	while (1)
	{
		pdu = recvMsg(sockfd);
		if (NULL == pdu)
		{
			printf("disconnected...\n");
			break;
		}
		switch (pdu->uiMsgType)
		{
		case ENUM_MSG_TYPE_GET_FRIEND_LIST_RESPOND:
			handleServerGetFriendListRespond(pdu);
			break;		
		case ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND:
			printf("server respond:%s\n", pdu->caMsg);
			break;
		case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
			printf("private chat: %d says:%s\n"
				   , pdu->uiFrom, pdu->caMsg);
			break;
		case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
			printf("group chat:%d says:%s\n", pdu->uiFrom, pdu->caMsg);
			break;
		}
		free(pdu);
		pdu = NULL;
	}
}

int main(void)
{
	//AF_INET:ipv4
	//SOCK_STREAM:可靠传输-->tcp
	//0:表示使用指定传输方式的默认协议
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
	int ret = connect(sockfd, (struct sockaddr *)&saddr
					  , sizeof(saddr));
	if (-1 == ret)
	{
		printf("connect::errno=%d, error=%s\n"
			   , errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("connect to server ok\n");

	int sign = 0;
	while (1)
	{
		ret = loginOrRegist();
		switch (ret)
		{
		case 1:
			sign = login(sockfd);
			break;
		case 2:
			regist(sockfd);
			break;
		case 0:
			break;
		default:
			printf("input error !\n");
			break;
		}
		if (1 == sign)
		{
			break;
		}
	}

	pthread_t thr;
	//创建线程专门处理服务器回复的信息
	pthread_create(&thr, NULL, handleServer, (void*)sockfd);
	//专门用来发送信息
	chat(sockfd);

	close(sockfd);	

	return 0;
}

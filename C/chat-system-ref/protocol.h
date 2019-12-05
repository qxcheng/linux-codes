#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned int uint;

enum ENUM_MSG_TYPE
{
	ENUM_MSG_TYPE_MIN = 0,
	ENUM_MSG_TYPE_REGIST_REQUEST,
	ENUM_MSG_TYPE_REGIST_RESPOND,
	
	ENUM_MSG_TYPE_LOGIN_REQUEST,
	ENUM_MSG_TYPE_LOGIN_RESPOND,
	
	ENUM_MSG_TYPE_GET_FRIEND_LIST_REQUEST,
	ENUM_MSG_TYPE_GET_FRIEND_LIST_RESPOND,
	
	ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,
	ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,
	
	ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,
	ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,
};

typedef struct PDU  //协议数据单元
{
	uint uiPDULen;     //数据的总大小
	uint uiMsgType;    //消息类型
	uint uiFrom;       //消息发送者
	uint uiTo;         //消息接收者
	uint uiMsgLen;     //实际消息的长度
	char caMsg[4];     //实际消息
}PDU;
//产生一块空间保存发送或者接收的信息
PDU *mkPDU(uint uiMsgLen);
//接收数据
PDU *recvMsg(int sockfd);
//发送数据
void sendMsg(const PDU *pdu, int sockfd);


#endif

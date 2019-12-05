#include "protocol.h"

/*
 char *p = (char *)malloc(0);
 free(p);
 */

//产生一块空间保存发送或者接收的信息
PDU *mkPDU(uint uiMsgLen)
{
	uint uiPDULen = sizeof(PDU)-4*sizeof(char)+uiMsgLen;
	PDU *pdu = (PDU*)malloc(uiPDULen);
	if (NULL == pdu)
	{
		printf("malloc pdu failed\n");
		exit(EXIT_FAILURE);
	}
	memset(pdu, 0, uiPDULen);
	//空间总的大小
	pdu->uiPDULen = uiPDULen;
	//实际消息的大小
	pdu->uiMsgLen = uiMsgLen;

	return pdu;
}

//接收数据
PDU *recvMsg(int sockfd)
{
	uint uiPDULen = 0;
	int ret = 0;
	//先接收前面4字节的数据，获得总的数据大小
	ret = read(sockfd, &uiPDULen, sizeof(uint));
	if (0 == ret)
	{
		printf("disconnected...\n");
		return NULL;
	}
	uint uiMsgLen = uiPDULen-(sizeof(PDU)-4*sizeof(char));
	PDU *pdu = mkPDU(uiMsgLen);
	uint uiLeft = uiPDULen-sizeof(uint);
	uint uiRecved = sizeof(uint);
	while (uiLeft)
	{
		if (uiLeft > 4096)
		{
			ret = read(sockfd, (char*)pdu+uiRecved, 4096);
		}
		else
		{
			ret = read(sockfd, (char*)pdu+uiRecved, uiLeft);
		}
		if (0 == ret || -1 == ret)
		{
			break;
		}
		uiLeft -= ret;
		uiRecved += ret;
	}
	return pdu;
}

//发送数据
void sendMsg(const PDU *pdu, int sockfd)
{
	if (NULL == pdu)
	{
		return;
	}
	uint uiLeft = pdu->uiPDULen;
	uint uiSended = 0;
	int ret = 0;
	while (uiLeft)
	{
		if (uiLeft > 4096)
		{
			ret = write(sockfd, (char*)pdu+uiSended
						, 4096);
		}
		else
		{
			ret = write(sockfd, (char*)pdu+uiSended
						, uiLeft);
		}
		if (0 == ret || -1 == ret)
		{
			break;
		}
		uiSended += ret;
		uiLeft -= ret;
	}
}






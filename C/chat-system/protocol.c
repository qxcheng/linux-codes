#include "public.h"
#include "protocol.h"

PDU *mkPDU(int uiMsgLen)
{
    int uiPDULen = sizeof(PDU) - 4*sizeof(char) + uiMsgLen;
    
    PDU *pdu = (PDU *)malloc(uiPDULen);
    if(NULL == pdu)
    {
        printf("pdu malloc failed.\n");
        return NULL;
    }
    memset(pdu, 0, uiPDULen);
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgLen = uiMsgLen;

    return pdu;
}

PDU *recvMsg(int sockfd)
{
    int uiPDULen = 0;

    int ret = read(sockfd, &uiPDULen, sizeof(int));
    if(ret == 0)
    {
        //printf("receive msg ...disconnected...\n");
        return NULL;
    }

    int uiMsgLen = uiPDULen - (sizeof(PDU) - 4*sizeof(char));
    PDU *pdu = mkPDU(uiMsgLen);

    int uiRecv = sizeof(int);
    int uiLeft = uiPDULen - uiRecv;
    
    while(uiLeft > 0)
    {
        if(uiLeft > 4096)
        {
            ret = read(sockfd, (char *)pdu + uiRecv, 4096);
        }
        else
        {
            ret = read(sockfd, (char *)pdu + uiRecv, uiLeft);
        }
        if(0 == ret || -1 == ret)
        {
            break;
        }
        uiRecv += ret;
        uiLeft -= ret;
    }
    return pdu;
}

void sendMsg(const PDU *pdu, int sockfd)
{
    if (NULL == pdu)
    {
        return ;
    }
    int uiLeft = pdu->uiPDULen;
    int uiSend = 0;
    int ret = 0;
    while(uiLeft > 0)
    {
        if(uiLeft > 4096)
        {
            ret = write(sockfd, (char *)pdu + uiSend, 4096);
        }
        else
        {
            ret = write(sockfd, (char *)pdu + uiSend, uiLeft);
        }
        if(0 == ret || -1 == ret)
        {
            break;
        }
        uiSend += ret;
        uiLeft -= ret;
    }
}

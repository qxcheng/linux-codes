#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum MSG_TYPE
{
    MSG_TYPE_MIN = 0,
    MSG_TYPE_REGIST_REQUEST,
    MSG_TYPE_REGIST_RESPOND,

    MSG_TYPE_LOGIN_REQUEST,
    MSG_TYPE_LOGIN_RESPOND,

    MSG_TYPE_CLIENT_PORT_REQUEST,

    MSG_TYPE_CLIENT_IP_PORT_REQUEST,
    MSG_TYPE_CLIENT_IP_PORT_RESPOND,

    MSG_TYPE_FRIENDLIST_REQUEST,
    MSG_TYPE_FRIENDLIST_RESPOND,

    MSG_TYPE_GROUP_REQUEST,
    MSG_TYPE_GROUP_RESPOND,

};

typedef struct PDU
{
    int uiPDULen;
    int uiMsgType;
    int uiFrom;
    int uiTo;
    int uiMsgLen;
    char usrName[128];
    char caMsg[4];
}PDU;

PDU *mkPDU(int uiMsgLen);

PDU *recvMsg(int sockfd);

void sendMsg(const PDU *pdu, int sockfd);


#endif

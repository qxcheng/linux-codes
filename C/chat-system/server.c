#include "public.h"
#include "protocol.h"
#include "mysql.h"
#include "list.h"
#include <sys/types.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>

/* global var declared *//*{{{*/
usrList *g_pList = NULL;
int g_maxFD = 0;/*}}}*/

/* function declared *//*{{{*/
void initSet(fd_set *rset);
void acceptClient(int sockfd);
void handleClientMsg(fd_set *rset);
void handleRegist(PDU *pdu, int csockfd);
void handleLogin(PDU *pdu, int csockfd);
void handlePort(PDU *pdu, int csockfd);
void handleIpPort(PDU *pdu, int csockfd);
void handleFriendList(int csockfd);
void handleGroup(PDU *pdu, int csockfd);/*}}}*/

int main()
{
    //TCP connect /*{{{*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("create socket");
        return 0;
    }

    int opt = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(ret < 0)
    {
        perror("setsockopt");
        return 0;
    }
    
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(8888);
    bzero(saddr.sin_zero,8);

    ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(-1 == ret)
    {
        perror("bind");
        return 0;
    }
    ret = listen(sockfd, 25);
    if(-1 == ret)
    {
        perror("listen");
        return 0;
    }/*}}}*/

    g_pList = mkUsrList();/*{{{*/
    g_pList->usrNewId = findMaxUsrId() + 1;

    fd_set rset;
    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(sockfd, &rset);
        g_maxFD = sockfd;
        initSet(&rset);
        
        ret = select(g_maxFD+1, &rset, NULL, NULL, NULL);
        if(ret < 0)
        {
            perror("select");
            break;
        }
        if(FD_ISSET(sockfd, &rset))
        {
            acceptClient(sockfd);
        }
        else
        {
            handleClientMsg(&rset);
        }
    }
}/*}}}*/

void initSet(fd_set *rset)/*{{{*/
{
    usrNode *pNode = g_pList->pHeadNode;
    while(pNode)
    {
        FD_SET(pNode->sockfd, rset);
        if(g_maxFD < pNode->sockfd)
        {
            g_maxFD = pNode->sockfd;
        }
        pNode = pNode->next;
    }
}/*}}}*/

void acceptClient(int sockfd)/*{{{*/
{
    struct sockaddr_in caddr;
    int cLen = sizeof(caddr);
    int csockfd = -1;

    csockfd = accept(sockfd, (struct sockaddr *)&caddr, &cLen);
    if(-1 == csockfd)
    {
        perror("accept");
        return ;
    }
    char c_ip[32];
    strcpy(c_ip, inet_ntoa(caddr.sin_addr));
    int c_port = ntohs(caddr.sin_port);
    printf("client connect: csockfd=%d, ip=%s, port=%d\n", csockfd, c_ip, c_port);

    usrNode *pNode = mkUsrNode();
    pNode->sockfd = csockfd;
    pNode->port = c_port;   
    strcpy(pNode->ip, c_ip);

    insertList(g_pList, pNode);
    
}/*}}}*/

void handleClientMsg(fd_set *rset)/*{{{*/
{
    usrNode *pNode = g_pList->pHeadNode;
    while(pNode)
    {
        if(FD_ISSET(pNode->sockfd, rset))
        {
            PDU *pdu = recvMsg(pNode->sockfd);
            if(NULL == pdu)
            {
                printf("client disconnect: csockfd=%d ip=%s port=%d\n", pNode->sockfd, pNode->ip, pNode->port);
                close(pNode->sockfd);
                usrNode *temp = pNode;
                pNode = pNode->next;
                deleteList(g_pList, temp->sockfd);
            }
            else
            {
                switch(pdu->uiMsgType)
                {
                    case MSG_TYPE_REGIST_REQUEST:
                        handleRegist(pdu, pNode->sockfd);
                        break;
                    case MSG_TYPE_LOGIN_REQUEST:
                        handleLogin(pdu, pNode->sockfd);
                        break;
                    case MSG_TYPE_CLIENT_PORT_REQUEST:
                        handlePort(pdu, pNode->sockfd);
                        break;
                    case MSG_TYPE_CLIENT_IP_PORT_REQUEST:
                        handleIpPort(pdu, pNode->sockfd);
                        break;
                    case MSG_TYPE_FRIENDLIST_REQUEST:
                        handleFriendList(pNode->sockfd);
                        break;
                    case MSG_TYPE_GROUP_REQUEST:
                        handleGroup(pdu,pNode->sockfd);
                        break;
                }
                pNode = pNode->next;
            }
            free(pdu);
            pdu = NULL;
        }
        else
        {
            pNode = pNode->next;
        }
    }
}/*}}}*/

void handleRegist(PDU *pdu, int csockfd)/*{{{*/
{
    char usrName[128];
    char usrPwd[32];
    strcpy(usrName, pdu->usrName);
    strcpy(usrPwd, pdu->caMsg);
    int usrId = g_pList->usrNewId;
    
    saveUsrInfo(usrId, usrPwd, usrName); //save to mysql
 
    (g_pList->usrNewId)++;  //user ID +1
    
    PDU *resPdu = mkPDU(0);
    resPdu->uiTo = usrId;
    resPdu->uiMsgType = MSG_TYPE_REGIST_RESPOND;
    sendMsg(resPdu, csockfd);

    free(resPdu);
    resPdu = NULL;
}/*}}}*/

void handleLogin(PDU *pdu, int csockfd)/*{{{*/
{
    int usrId = pdu->uiFrom;
    char usrPwd[32] = {'\0'};
    strcpy(usrPwd, pdu->caMsg);

    PDU *respdu = NULL;

    int ret = loginCheck(usrId, usrPwd);
    if(ret)
    {
        respdu = mkPDU(strlen("login ok") + 1);
        strcpy(respdu->caMsg, "login ok");

        usrNode *pNode = g_pList->pHeadNode;
        while(pNode && pNode->sockfd != csockfd)
        {
            pNode = pNode->next;
        }
        pNode->usrId = usrId;
        findUsrName(usrId, pNode->usrName);  
    }
    else
    {
        respdu = mkPDU(strlen("login error") + 1);
        strcpy(respdu->caMsg, "login error");
    }
    respdu->uiMsgType = MSG_TYPE_LOGIN_RESPOND;
    respdu->uiTo = usrId;
    sendMsg(respdu, csockfd);
    free(respdu);
    respdu = NULL;
}/*}}}*/

void handlePort(PDU *pdu, int csockfd)/*{{{*/
{
    usrNode *pNode = g_pList->pHeadNode;
    while(pNode && pNode->sockfd != csockfd)
    {
        pNode = pNode->next;
    }
    pNode->u_port = pdu->uiFrom;
}/*}}}*/

void handleIpPort(PDU *pdu, int csockfd)/*{{{*/
{
    int usrId = pdu->uiFrom;
    usrNode *pNode = findUsrNode(g_pList, usrId);

    PDU *respdu = mkPDU(strlen(pNode->ip) + 1);
    respdu->uiMsgType = MSG_TYPE_CLIENT_IP_PORT_RESPOND;
    respdu->uiTo = pNode->u_port;
    strcpy(respdu->caMsg, pNode->ip);

    sendMsg(respdu, csockfd);
    free(respdu);
    respdu = NULL;
}/*}}}*/

void handleFriendList(int csockfd)/*{{{*/
{
    char caMsg[1024] = {'\0'};
    char temp[32] = {'\0'};
    usrNode *pNode = g_pList->pHeadNode;
    while(pNode)
    {
        if(pNode->usrId)
        {
            sprintf(temp, "%d", pNode->usrId);
            strcat(caMsg, temp);
            strcat(caMsg, " ");
            strcat(caMsg, pNode->usrName);
            strcat(caMsg, "\n");
        }
        pNode = pNode->next;
    }
    PDU *pdu = mkPDU(strlen(caMsg) + 1);
    pdu->uiMsgType = MSG_TYPE_FRIENDLIST_RESPOND;
    strcpy(pdu->caMsg, caMsg);
    sendMsg(pdu, csockfd);
    free(pdu);
    pdu = NULL;
}/*}}}*/

void handleGroup(PDU *pdu, int csockfd)/*{{{*/
{
    char caMsg[1024];
    sprintf(caMsg, "%d:", pdu->uiFrom);
    strcat(caMsg, pdu->caMsg);

    PDU *respdu = mkPDU(strlen(caMsg)+1);
    respdu->uiMsgType = MSG_TYPE_GROUP_RESPOND;
    strcpy(respdu->caMsg, caMsg);

    int usockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == usockfd)
    {
        return;
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    bzero(saddr.sin_zero, 8);

    usrNode *pNode = g_pList->pHeadNode;
    while(pNode)
    {
        if(pNode->sockfd != csockfd)
        {

            saddr.sin_addr.s_addr = inet_addr(pNode->ip);
            saddr.sin_port = htons(pNode->u_port);
            sendto(usockfd,caMsg,strlen(caMsg),0,(struct sockaddr*)&saddr,sizeof(saddr));
        }
        pNode = pNode->next;
    }
    close(usockfd);
            
    free(respdu);
    respdu = NULL;
    
}/*}}}*/



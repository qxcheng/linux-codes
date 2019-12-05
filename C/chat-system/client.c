#include "public.h"
#include "protocol.h"
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* function declared */
void regist(int sockfd);/*{{{*/
int login(int sockfd);
void *handleServer(void *arg);
void chat(int sockfd);
void privateChat(int sockfd);
void getFriendList(int sockfd);
void groupChat(int sockfd);
/*}}}*/

int g_usrId;

int main()
{
    //TCP Connect/*{{{*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("create socket");
        return 0;
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(8888);
    bzero(saddr.sin_zero,8);
    int ret = connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(-1 == ret)
    {
        perror("connect");
        return 0;
    }
    printf("Connect to server successful!\n");/*}}}*/

    int flag = 0;/*{{{*/

    int key;
    while(1)
    {
        system("clear");
        printf("===Welcome===\n");
        printf(" 1.login\n");
        printf(" 2.regist\n");
        printf(" 0.exit\n");
        printf("=============\n");
        scanf("%d", &key);        
        switch(key)
        {
            case 1:
                flag = login(sockfd);
                break;
            case 2:
                regist(sockfd);
                break;
            case 0:
                close(sockfd);
                exit(0);
                break;
            default:
                printf("input error~\n");
                getchar();
                getchar();
                getchar();
                break;
        }
        if(flag)
        {
            break;
        }
    }
    
    pthread_t thr;
    /* create udp server to recv private msg */
    pthread_create(&thr, NULL, handleServer, (void *)sockfd);


    chat(sockfd);

    close(sockfd);
    return 0;
}/*}}}*/

void regist(int sockfd)/*{{{*/
{
    char usrName[128] = {'\0'};
    char usrPwd[32] = {'\0'};
    printf("Please set your name:\n");
    scanf("%s", usrName);
    printf("Please set your password:\n");
    scanf("%s", usrPwd);

    PDU *pdu = mkPDU(strlen(usrPwd) + 1);
    strcpy(pdu->usrName, usrName);
    strcpy(pdu->caMsg, usrPwd);
    pdu->uiMsgType = MSG_TYPE_REGIST_REQUEST;

    sendMsg(pdu, sockfd);
    free(pdu);
    pdu = NULL;

    pdu = recvMsg(sockfd);
    if(NULL == pdu)
    {
        return ;
    }
    if(pdu->uiMsgType == MSG_TYPE_REGIST_RESPOND)
    {
        printf("your id = %d\n", pdu->uiTo);
        getchar();
        getchar();
    }
    free(pdu);
    pdu = NULL;

} /*}}}*/
    
int login(int sockfd)/*{{{*/
{
    int flag=0;
    int usrId;
    char usrPwd[32] = {'\0'};
    printf("Please input your id:\n");
    scanf("%d", &usrId);
    printf("Please input your password:\n");
    scanf("%s", usrPwd);

    PDU *pdu = mkPDU(strlen(usrPwd) + 1);
    pdu->uiFrom = usrId;
    strcpy(pdu->caMsg, usrPwd);
    pdu->uiMsgType = MSG_TYPE_LOGIN_REQUEST;
    
    sendMsg(pdu, sockfd);
    free(pdu);
    pdu = NULL;

    pdu = recvMsg(sockfd);
    if(NULL == pdu)
    {
        return -1;
    }
    if(pdu->uiMsgType == MSG_TYPE_LOGIN_RESPOND)
    {
        printf("login respond: %s\n", pdu->caMsg);
        if(0 == strcmp(pdu->caMsg, "login ok"))
        {
            flag = 1;
            g_usrId = pdu->uiTo;
        }
    }
    free(pdu);
    pdu = NULL;
    return flag;

}/*}}}*/

void chat(int sockfd)/*{{{*/
{
    int key;
    while(1)
    {
        system("clear");
        printf("=====Select=====\n");
        printf("1.private chat\n");
        printf("2.friend online\n");
        printf("3.group chat\n");
        printf("0.exit\n");
        printf("================\n");
        scanf("%d", &key);        
        switch(key)
        {
            case 1:
                privateChat(sockfd);
                break;
            case 2:
                getFriendList(sockfd);
                break;
            case 3:
                groupChat(sockfd);
                break;
            case 0:
                close(sockfd);
                exit(0);
                break;
        }
    }
}/*}}}*/

void privateChat(int sockfd)/*{{{*/
{
    int usrId;
    printf("Please select a user ID:\n");
    scanf("%d", &usrId);

    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = MSG_TYPE_CLIENT_IP_PORT_REQUEST;
    pdu->uiFrom = usrId;

    sendMsg(pdu, sockfd);
    free(pdu);
    pdu = NULL;
    
    pdu = recvMsg(sockfd);
    if(NULL == pdu)
    {
        return ;
    }
    char ip[32] = {'\0'};
    strcpy(ip, pdu->caMsg);
    int port = pdu->uiTo;

    char caMsg[1024];
    sprintf(caMsg, "%d:", g_usrId);

    char temp[1024] = {'\0'};

    printf("Please input your message:\n");
    getchar();
    gets(temp);
    strcat(caMsg, temp);

    int usockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == usockfd)
    {
        perror("socket");
        return ;
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(port);
    bzero(saddr.sin_zero, 8);
    
    sendto(usockfd, caMsg, strlen(caMsg), 0, (struct sockaddr *)&saddr, sizeof(saddr));

    close(usockfd);
    free(pdu);
    pdu = NULL;
}/*}}}*/

void getFriendList(int sockfd)/*{{{*/
{
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = MSG_TYPE_FRIENDLIST_REQUEST;
    sendMsg(pdu, sockfd);
    free(pdu);
    pdu = NULL;

    pdu = recvMsg(sockfd);
    if(NULL == pdu)
    {
        return ;
    }
    if(pdu->uiMsgType == MSG_TYPE_FRIENDLIST_RESPOND)
    { 
        printf("%s\n", pdu->caMsg);
    }
    getchar();
    getchar();
    free(pdu);
    pdu = NULL;
}/*}}}*/

void groupChat(int sockfd)/*{{{*/
{
    char caMsg[1024];
    printf("Please input your message:\n");
    getchar();
    gets(caMsg);

    PDU *pdu = mkPDU(strlen(caMsg) + 1);
    pdu->uiMsgType = MSG_TYPE_GROUP_REQUEST;
    strcpy(pdu->caMsg, caMsg);
    pdu->uiFrom = g_usrId;
    sendMsg(pdu, sockfd);
    free(pdu);
    pdu = NULL;

}/*}}}*/

void *handleServer(void *arg)/*{{{*/
{
    int sockfd = (int)arg;
    
    time_t t;
    srand((unsigned)time(&t));
    int u_port = rand() % 10000;

    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = MSG_TYPE_CLIENT_PORT_REQUEST;
    pdu->uiFrom = u_port;   
    sendMsg(pdu, sockfd);
    free(pdu);
    pdu = NULL;
    
    int usockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == usockfd)
    {
        perror("sockfd");
        return NULL;
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(u_port);
    bzero(saddr.sin_zero, 8);
    int ret = bind(usockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(-1 == ret)
    {
        perror("bind");
        return NULL;
    }
    struct sockaddr_in caddr;
    int cLen = sizeof(caddr);
    char caMsg[1024] = {'\0'};
    while(1)
    {
        memset(caMsg, 0, sizeof(caMsg));
        recvfrom(usockfd, caMsg, sizeof(caMsg), 0, (struct sockaddr*)&caddr, &cLen);
        printf("Recvmsg from %s\n",  caMsg);
    }
}
/*}}}*/








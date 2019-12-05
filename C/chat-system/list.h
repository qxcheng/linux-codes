#ifndef __LIST_H__
#define __LIST_H__

typedef struct usrNode
{
    int usrId;
    int sockfd;
    int port;
    int u_port;
    char ip[32];
    char usrName[128];

    struct usrNode *next;
}usrNode;

typedef struct usrList
{
    int usrLen;
    int usrNewId;
    usrNode *pHeadNode;
}usrList;

usrNode *mkUsrNode();
usrList *mkUsrList();

void insertList(usrList *, usrNode *);
void deleteList(usrList *, int sockfd);
usrNode *findUsrNode(usrList *, int usrId);


#endif

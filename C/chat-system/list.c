#include "public.h"
#include "list.h"

usrNode *mkUsrNode()
{
    usrNode *pNode = NULL;
    pNode = (usrNode *)malloc(sizeof(usrNode));
    if(NULL == pNode)
    {
        printf("malloc usernode failed.\n");
        exit(EXIT_FAILURE);
    }
    memset(pNode, 0, sizeof(usrNode));
    return pNode;
}

usrList *mkUsrList()
{
    usrList *pList;
    pList = (usrList *)malloc(sizeof(usrList));
    if(NULL == pList)
    {
        printf("malloc userlist failed.\n");
        exit(EXIT_FAILURE);
    }
    memset(pList, 0, sizeof(usrList));
    return pList;
}
  
void insertList(usrList *pList, usrNode *pNode)
{
    if(NULL == pList || NULL == pNode)
    {
        return ;
    }
    pNode->next = pList->pHeadNode;
    pList->pHeadNode = pNode;
    (pList->usrLen)++;
}

void deleteList(usrList *pList, int sockfd)
{
    if(NULL == pList || sockfd < 0)
    {
        return ;
    }
    usrNode *pNode = pList->pHeadNode; 
    if(pNode->sockfd == sockfd)
    {
        pList->pHeadNode = pNode->next;
        free(pNode);
        (pList->usrLen)--;
    }
    else
    {
        while(pNode->next && pNode->next->sockfd != sockfd)
        {
            pNode = pNode->next;
        }
        if(pNode->next->next != NULL)
        {
            usrNode *temp = pNode->next;
            pNode->next = temp->next;
            free(temp);
            (pList->usrLen)--;
        }
        else
        {
            usrNode *temp = pNode->next;
            pNode->next = NULL;
            free(temp);
            (pList->usrLen)--;
        }
    }
}

usrNode *findUsrNode(usrList *pList, int usrId)
{
    usrNode *pNode = pList->pHeadNode;
    while(pNode && pNode->usrId != usrId)
    {
        pNode = pNode->next;
    }
    return pNode;
}

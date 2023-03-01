#ifndef USR_LINK_H
#define USR_LINK_H

#include "public.h"

typedef struct UsrNode
{
	int sockfd;
	struct UsrNode *pNext;
}UsrNode;

typedef struct UsrList
{
	uint uiLen;
	UsrNode *pFirstNode;
}UsrList;

UsrNode *mkUsrNode();
UsrList *mkUsrList();
void insertUsrList(UsrList *list, UsrNode *node);

#endif

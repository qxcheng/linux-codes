#ifndef USR_LINK_H
#define USR_LINK_H

#include "public.h"

typedef struct UsrNode
{
	uint uiId;
	char caPwd[32];
	int sockfd;

	struct UsrNode *pNext;
}UsrNode;

typedef struct UsrList
{
	uint uiNewId;
	uint uiLen;
	UsrNode *pFirstNode;
}UsrList;

UsrNode *mkUsrNode();
UsrList *mkUsrList();
void insertUsrList(UsrList *list, UsrNode *node);
UsrNode *findUsrById(const UsrList *list, uint iId);

#endif

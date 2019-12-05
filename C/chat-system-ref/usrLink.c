#include "usrLink.h"

UsrNode *mkUsrNode()
{
	UsrNode *node = (UsrNode *)malloc(sizeof(UsrNode));
	if (NULL == node)
	{
		printf("malloc usr node failed\n");
		exit(EXIT_FAILURE);
	}
	memset(node, 0, sizeof(UsrNode));
	return node;
}

UsrList *mkUsrList()
{
	UsrList *list = (UsrList *)malloc(sizeof(UsrList));
	if (NULL == list)
	{
		printf("malloc usr list failed\n");
		exit(EXIT_FAILURE);
	}
	memset(list, 0, sizeof(UsrList));
	list->uiNewId = 1000;
	return list;
}

void insertUsrList(UsrList *list, UsrNode *node)
{
	if (NULL == list || NULL == node)
	{
		return;
	}

	node->pNext = list->pFirstNode;
	list->pFirstNode = node;
	list->uiLen++;
	if (list->uiNewId <= node->uiId)
	{
		list->uiNewId = node->uiId+1;
	}
}

UsrNode *findUsrById(const UsrList *list, uint iId)
{
	if (NULL == list)
	{
		return NULL;
	}

	UsrNode *node = list->pFirstNode;
	while (NULL != node)
	{
		if (iId == node->uiId)
		{
			break;
		}
		node = node->pNext;
	}

	return node;
}






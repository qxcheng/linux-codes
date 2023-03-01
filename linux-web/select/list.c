#include "list.h"

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
}

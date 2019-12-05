#include "fileope.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define USR_INFO_PATH "./usr.info"

int openFile(const char *path, int flags)
{
	if (NULL == path)
	{
		printf("file path error\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(path, flags|O_CREAT);
	if (-1 == fd)
	{
		printf("file path error\n");
		exit(EXIT_FAILURE);
	}
	return fd;
}

void saveUsrInfoToFile(const UsrList *list)
{
	if (NULL == list)
	{
		return;
	}

	int fd = openFile(USR_INFO_PATH, O_WRONLY|O_TRUNC);
	UsrNode *node = list->pFirstNode;
	while (NULL != node)
	{
		write(fd, node, sizeof(UsrNode));
		node = node->pNext;
	}
	close(fd);
}

void getUsrInfoFromFile(UsrList *list)
{
	if (NULL == list)
	{
		return;
	}

	UsrNode *node = NULL;
	int fd = openFile(USR_INFO_PATH, O_RDONLY);
	while (1)
	{
		node = mkUsrNode();
		if (0 == read(fd, node, sizeof(UsrNode)))
		{
			free(node);
			node = NULL;
			break;
		}
		node->pNext = NULL;
		node->sockfd = 0;
		insertUsrList(list, node);
	}
	close(fd);
}








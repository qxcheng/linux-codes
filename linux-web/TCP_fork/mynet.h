#ifndef __MYNET_H
#define __MYNET_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define RET_OK 0
#define RET_ERR -1

#define LISTEN_QUEUE_NUM 5
#define BUFFER_SIZE 256
#define ECHO_PORT 2029

#endif /*__MYNET_H*/
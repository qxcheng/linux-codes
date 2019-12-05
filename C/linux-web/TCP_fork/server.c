#include "mynet.h"

//自定以信号注册函数
static void (*set_signal(int signo,void (*func)(int)))(int)
{
	struct sigaction act,oact;
	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo != SIGALRM)
		act.sa_flags|=SA_RESTART;
	if(sigaction(signo,&act,&oact)<0)
		return NULL;
	return(oact.sa_handler);
}
	
//信号处理函数
static void sig_routine(int signo)
{
	switch(signo)
	{
		case SIGCHLD:
			while(waitpid(-1, NULL, WNOHANG) > 0);
			break;
	}
	return;
}

//读取一行
static int my_readline(int fd, void *buf, unsigned len)
{
	int n, rc;
	char c, *ptr;
	ptr = buf;
	for (n = 1; n < len; n++) {
again:
		if ( (rc = read(fd, &c, 1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} 
		else if (rc == 0) {
			if (n == 1)
				return(0);
			else
				break;
		} 
		else {
			if (errno == EINTR)
				goto again;
			return(-1);
		}
	}
	*ptr = 0;
	return(n);
}

//处理客户端消息
int proc_echo(int consock)
{
	int ret = 0;
	char buffer[BUFFER_SIZE]={0};
	while(1)
	{ 
		memset(buffer,0,sizeof(buffer));
		if((ret = read(consock, buffer, sizeof(buffer)-1)) < 0)
		{
			perror("read");
			break;
		}
		else if(ret == 0)
		{
			printf("client disconnect.\n");
			break;
		}
		if((ret = write(consock, buffer, ret)) < 0)
		{
			perror("write");
			break;
		}
	}
	close(consock);
	return ret;
}

int main(int argc, char *argv[])
{
	int sockfd, nsock;
	uint32_t len;
	pid_t pid;
	struct sockaddr_in servaddr, cliaddr;
	int ret = RET_OK;  // 0
	
	//建立TCP连接
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("ERROR opening socket");
		return RET_ERR;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY; //监听所有IP地址
	servaddr.sin_port = htons(ECHO_PORT);
	if ((ret = bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))) < 0)
	{
		perror("ERROR on binding");
		goto failed;
	}
	if((ret = listen(sockfd,LISTEN_QUEUE_NUM)) != 0)
	{
		perror("ERROR on listen");
		goto failed;
	}
	
	//安装信号
	if((set_signal(SIGCHLD, sig_routine)) < 0) 
	{
		perror("ERROR on set_signal");
		goto failed;
	}
	
	while(1)
	{
		len = sizeof(cliaddr);
		if((ret = accept(sockfd, (struct sockaddr *) &cliaddr, &len)) <
	0)
		{
			if(errno == EINTR)  //信号中断处理
				continue;
			perror("accept");   //错误处理
			break;
		}
		
		nsock = ret;  //客户端套接字
		
		if((pid = fork()) < 0)
		{
			break;
		}
		else if(pid == 0)
		{
			close(sockfd);  //子进程关闭监听套接字
			return proc_echo(nsock);  //返回同时调用函数
		}
		close(nsock);  //父进程关闭客户端套接字，继续监听
	}
	
failed:
	close(sockfd);
	return ret < 0 ? RET_ERR : RET_OK;
	
}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <fcntl.h>

/* 用于修改套接字为非阻塞 */
/*
void setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL);
	if(opts<0)
	{
		perror("fcntl GETFL");
		exit(1);
	}
	opts=opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		perror("fcntl SETFL");
		exit(1);
	}
}
*/

int main()
{
	int i;
	int ret =0;
	char buf[1024]={0};
	
	int listenfd=0; //listen socket file
	int connfd=0;   //connected socket file
	
	uint32_t len = 0;
	struct sockaddr_in servaddr={0};
	struct sockaddr_in cliaddr={0};
	
	int epoll_instance,nfound,sockfd;
	
	/* ev用于注册事件, events数组用于回传要处理的事件*/
	struct epoll_event ev,events[20];
	
	/*生成用于处理accept的epoll专用的文件描述符，指定生成描述符的最大范围为256*/
	epoll_instance=epoll_create(256);//size is unused nowadays
	
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd <0)
	{
		perror("error opening socket");
		return -1;
	}
	//setnonblocking(listenfd);//把用于监听的socket设置成非阻塞方式
	
	ev.data.fd=listenfd;//设置与要处理的事件相关的文件描述符
	ev.events=EPOLLIN|EPOLLET;//设置要处理的事件类型
	epoll_ctl(epoll_instance,EPOLL_CTL_ADD,listenfd,&ev);//注册epoll事件
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons((uint16_t)20000);
	
	ret = bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(ret <0)
	{
		perror("Error on binding");
		return -1;
	}
	ret = listen(listenfd,5);//backlog
	if(ret !=0)
	{
		perror("Error on listening");
		return -1;
	}
	
	while(1)
	{
		nfound=epoll_wait(epoll_instance,events,20,1000);//等待epoll事件的发生
		if(nfound==0)
		{
			printf(".");
			fflush(stdout);
			continue;
		}
		for(i=0;i<nfound;i++)
		{
			if(events[i].data.fd==listenfd)/*监听事件*/
			{
				len = sizeof(cliaddr);
				connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
				//返回值待处理
				
				printf("connection from host %s,port %d,sockfd %d\n",
				inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port),connfd);
				
				//setnonblocking(connfd);
				
				ev.data.fd=connfd;//设置用于读操作的文件描述符
				ev.events=EPOLLIN|EPOLLET;//设置用于注册的读操作事件
				epoll_ctl(epoll_instance,EPOLL_CTL_ADD,connfd,&ev);//注册ev事件
			} 
			else if(events[i].events & EPOLLIN)/*读事件*/
			{
				sockfd=events[i].data.fd;
				memset(buf,0,sizeof(buf));
				ret=read(sockfd,buf,sizeof(buf)-1);
				if(ret==0) //断开连接后的处理
				{
					ev.data.fd=sockfd;
					ev.events=EPOLLIN|EPOLLET;//
					epoll_ctl(epoll_instance,EPOLL_CTL_DEL,sockfd,&ev);
					close(sockfd);
					printf("del client\n");
					continue;
				}
				write(sockfd,buf,ret);
				printf("write back to client.....\n");
			}
		}
	}
}
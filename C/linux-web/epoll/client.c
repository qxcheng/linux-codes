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
#include <sys/epoll.h>
#include <fcntl.h>

int main()
{
	int i;
	int ret =0;
	unsigned char buf[1024]={0};
	
	int sockfd=0;
	struct sockaddr_in servaddr={0};
	int epoll_instance,nfound;
	
	/* ev用于注册事件, events数组用于回传要处理的事件 */
	struct epoll_event ev,events[20];
	
	/*生成用于处理accept的epoll专用的文件描述符，指定生成描述符的最大范围为256*/
	epoll_instance=epoll_create(256);//size is unused nowadays
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd <0)
	{
		perror("error opening socket");
		goto failed;
	}
	ev.data.fd=sockfd;//添加socket
	ev.events=EPOLLIN|EPOLLET;//设置要处理的事件类型
	epoll_ctl(epoll_instance,EPOLL_CTL_ADD,sockfd,&ev);//注册epoll事件
	
	ev.data.fd=fileno(stdin);//添加键盘
	ev.events=EPOLLIN|EPOLLET;//设置要处理的事件类型
	epoll_ctl(epoll_instance,EPOLL_CTL_ADD,fileno(stdin),&ev);//注册epoll事件
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr.s_addr);
	servaddr.sin_port = htons((uint16_t)20000);
	
	ret = connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(ret <0)
	{
		perror("Error connecting");
		goto failed;
	}
	
	while(1)
	{
		nfound=epoll_wait(epoll_instance,events,20,-1);//等待epoll事件的发生
		for(i=0;i<nfound;i++)
		{
			if(events[i].events&EPOLLIN && events[i].data.fd == fileno(stdin))/*监听事件*/
			{
				memset(buf,0,sizeof(buf));
				fgets(buf,sizeof(buf)-1,stdin);
				printf("writing socket\n");
				write(sockfd,buf,strlen(buf));
			}
			else if(events[i].events & EPOLLIN)/*读事件*/
			{
				//sockfd=events[i].data.fd;
				memset(buf,0,sizeof(buf));
				ret=read(sockfd,buf,sizeof(buf)-1);
				if(ret==0)  //连接断开处理
				{
					ev.data.fd=sockfd;
					ev.events=EPOLLIN|EPOLLET;//
					epoll_ctl(epoll_instance,EPOLL_CTL_DEL,sockfd,&ev);
					close(sockfd);
					printf("del client\n");
					continue;
				}
				printf("echo buf is %s\n",buf);
			}
		}
	}
	
failed:
	close(sockfd);
	
}
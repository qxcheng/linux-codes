#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>  //inet_addr
#include<arpa/inet.h>

struct student
{
    int id;
    char name[100];
    float score;
};

int main(void)
{
    //AF_INET:ipv4
    //SOCK_STREAM:可靠传输-->tcp
    //0:指定传输方式的默认协议
    int sockfd = socket(AF_INET,SOCK_STREAM,0);    
    if (-1 == sockfd)
    {
        printf("errno=%d, error=%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET; //ipv4
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");//transform
    saddr.sin_port = htons(8888); //bytes sequence transform
    bzero(saddr.sin_zero,8);//指定空间置为0
	
    int ret = connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(-1 == ret)
    {
        printf("connect:errno=%d, error=%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);        
    }
    printf("connect to server successful\n");

    int num = 0;
    char caMsg[100] = {'\0'};
    struct student stu;
    for(int i=0; i<10; i++)
    {
        stu.id = 1001+i;
        stu.score = 100-i;
        strcpy(stu.name,"ling");
        write(sockfd, &stu, sizeof(stu));//往服务器发送信息

        read(sockfd,caMsg,sizeof(caMsg));//阻塞等待接受服务器消息
        if(strcmp(caMsg,"OK") == 0)
        {
            num++;
            printf("Send %d msg successful\n",num);
            memset(caMsg,'\0',sizeof(caMsg));
        }
    }
    struct student s = {0, "exit", 0};
    write(sockfd,&s,sizeof(struct student));
    if(10 == num)
        printf("\nAll msg send successful\n");
    else
        printf("\n%d send successful, %d send fail\n",num,10-num);

    close(sockfd);


    return 0;
}

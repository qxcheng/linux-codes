#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>  //inet_addr
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/stat.h>

FILE *fp = NULL;
pthread_mutex_t mutex;

struct student
{
    int id;
    char name[100];
    float score;
};

void *handleClient(void *arg)
{
    int sockfd = (int)arg;
    struct student stu;

    while(1)
    {
        read(sockfd,&stu,sizeof(struct student));
        if(0 == stu.id && 0 == strcmp("exit",stu.name))
            break;
        pthread_mutex_lock(&mutex);
        fprintf(fp,"id:%d, name:%s, score:%.2f\n",stu.id,stu.name,stu.score);
        fclose(fp);
        fp = fopen("data", "a+");
        pthread_mutex_unlock(&mutex);
        write(sockfd,"OK",strlen("OK"));
    }
    return NULL;

}

int main(void)
{
    fp = fopen("data", "a+");
	
    //AF_INET:ipv4
    //SOCK_STREAM:可靠传输-->tcp
    //0:指定传输方式的默认协议
    //用于监听客户端连接
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
	
    //设置要监听的ip和port
    int ret = bind(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(-1 == ret)
    {
        printf("bind:errno=%d, error=%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);        
    }
	
    //将sockfd设置处于监听状态，并设置连接的缓存队列
    ret = listen(sockfd,25);
    if(-1 == ret)
    {
        printf("listen:errno=%d, error=%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);        
    }
	
    //caddr:用于保存客户端的ip,port
    struct sockaddr_in caddr;
    int iLen = sizeof(caddr);
    int csockfd = -1;
	
    char caMsg[128] = "OK";
    struct student stu;
    int num = 0;

    pthread_t thr;
    while(1)
    {
        printf("accept client...\n");
        //accept:阻塞等待客户端连接
        //成功连接时，会自动将客户端ip,port信息填入caddr中，
        //并会产生一个新的socket,用于服务器可客户端进行数据交互
        csockfd = accept(sockfd,(struct sockaddr *)&caddr,&iLen);
        if(-1 == csockfd)
        {
            perror("accept\n");
            continue;
        }

        printf("csockfd=%d, c_ip=%s, c_port=%d\n",csockfd,inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));

        //每来一个客户端都产生一个线程专门处理
        pthread_create(&thr, NULL, handleClient, (void *)csockfd);


       // close(csockfd);
    }
    

    return 0;
}

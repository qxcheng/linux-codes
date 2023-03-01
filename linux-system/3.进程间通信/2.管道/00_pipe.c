#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


//信号处理函数
void fun(int sig) { 
    wait(NULL); 
}

int main()
{
    char write_buf[1024], read_buf[1024];
    
    //创建管道
    int fds[2];              
    int ret = pipe(fds);   
    if(ret==-1){ 
        perror("pipe error:");
        exit(1); 
    }
    
    pid_t pid = fork();
    //父进程向管道写  
    if(pid > 0)              
    {             
        close(fds[0]);       //关闭读端
        signal(SIGPIPE,fun); //若读端异常关闭会发送SIGPIPE信号，在此捕捉处理
        while(1){
            //printf("Please input:\n");
            fgets(write_buf, sizeof(write_buf), stdin);
            write(fds[1], write_buf, strlen(write_buf));
        }
        close(fds[1]);
    }
    //子进程从管道读 
    else if(pid == 0)        
    {      
        close(fds[1]);       //关闭写端
        while(1){
            int num = read(fds[0],read_buf,sizeof(read_buf));
            if(0 == num){    //为0表示写端关闭，进行异常处理
                perror("pipe broken\n");
                break;
            }
            read_buf[num] = '\0';
            printf("read_buf=%s\n",read_buf);
        }
        close(fds[0]);
    }
    else{ 
        perror("fork error:");
        exit(1); 
    }
}
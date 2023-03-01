#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void)
{
    int fds[2];
    int ret = pipe(fds); 
    if (ret == -1) { 
        perror("pipe error:");
        exit(1);
    }

    int i;
    pid_t pid;
    for(i = 0; i < 2; i++){
        pid = fork();
        if (pid == -1){
            perror("pipe error:");  
            exit(1);
        }
        if(pid == 0)
            break;
    }
    if (i == 0)       //子进程1  执行ls 
    {       
        close(fds[0]);                //关闭读端
        dup2(fds[1], STDOUT_FILENO);  //标准输出描述符重定向到写端
        execlp("ls", "ls", NULL);     //执行ls，内容输出到写端
    } 
	else if (i == 1)  //子进程2  执行wc -l 
    {  
        close(fds[1]);                  //关闭写端
        dup2(fds[0], STDIN_FILENO);     //标准输入描述符重定向到读端
        execlp("wc", "wc", "-l", NULL); //执行wc, 从读端读取数据并统计行数
    } 
	else if (i == 2) //父进程关闭读写端 回收子进程
    {  
        close(fds[0]);
        close(fds[1]);
        for(i = 0; i < 2; i++)
            wait(NULL);
    }
    return 0;
}
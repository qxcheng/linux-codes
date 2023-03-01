#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(void)
{
    int fd[2], nbytes;
    pid_t childpid;
    char write_buf[] = "Hello world!\n";

    int ret = pipe(fd);   
    if(ret==-1){ 
        perror("pipe error:");
        exit(1); 
    }

    if((childpid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if(childpid == 0){
        close(fd[1]); // 关闭写端
        close(0);     // 关闭标准输入描述符
        dup(fd[0]);   // 标准输入描述符重定向到管道的读端
        execlp("wc", "wc", "-w", NULL); // 标准输入从键盘设备读转到从管道读
    } else{
        close(fd[0]);  // 关闭读端
        
        close(1);      // 关闭标准输出描述符
        dup(fd[1]);    // 标准输出描述符重定向到管道的写端
        //dup2(fd[1], STDOUT_FILENO);   
        
        //write(fd[1], write_buf, strlen(write_buf));
        printf("Write string: %s\n", write_buf); // 标准输出从屏幕设备转到管道写端
    }

    return(0);
}



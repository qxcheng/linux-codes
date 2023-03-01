#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, char* argv[], char** environ)
{
    char* str = "from parent's message";
    int stat;
    pid_t pid;
    int fd[2];

    pipe(fd);

    pid = fork();
    if(0 == pid)//child read
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        
        execve("myprocess", argv, environ);

    }
    else//parent write
    {

        close(fd[0]);
        int old = dup(STDOUT_FILENO);
        int new = dup2(fd[1], STDOUT_FILENO);

        
        write(fd[1], str, strlen(str)+1);

        dup2(old, new);//恢复重定向

        wait(&stat);//
        if ( WIFEXITED(stat) )
        {
            printf("child exited with code:%d\n", WEXITSTATUS(stat));
        }

        close(fd[1]);

        exit(0);

    }
    
}
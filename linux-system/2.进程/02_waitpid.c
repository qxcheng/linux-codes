#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>


int main()
{
    int n = 5, i;	
    int status;
    pid_t pid, wpid;
   
    for(i = 0; i < n; i++){
        pid = fork();
        if(pid == 0) {
            break;			
        } 
    }

    if(n == i) // parent
    {  
        sleep(n);
        printf("I am parent, pid = %d\n", getpid());
        for (i = 0; i < n; i++) 
        {
            do {
                wpid = waitpid(0, &status, WNOHANG);				
                if(wpid == 0){
                    printf("NO child exited\n");
                    sleep(1);		
                }
            } while (wpid == 0);	//子进程未回收
    
            if(wpid > 0){		    //回收了任意子进程
                printf("I'm parent, I catched child process pid = %d\n", wpid);
                if (WIFEXITED(status))
                    printf("Child exited with code %d\n", WEXITSTATUS(status));
                else if (WIFSIGNALED(status))
                    printf("Child terminated abnormally, signal %d\n", WTERMSIG(status));
            } 
            else {
                printf("error...\n");
            } 
        }
    } 
    else   //child
    {         
        sleep(i);
        printf("I'm %dth child, pid = %d\n", i+1, getpid());
    }
   
    return 0;
}
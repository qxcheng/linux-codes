#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


void p_action(), c_action();
int ntimes=0;

main()
{
    int pid, ppid;
    
    signal(SIGUSR1, p_action);   /* 设定父进程的 SIGUSR1 */
    switch(pid=fork()) {
        /*fork 失败*/
        case -1: 
            perror("synchro");
            exit(1);
        /*子进程模块*/
        case 0: 
            signal(SIGUSR1,c_action);  /* 设定子进程的 SIGUSR1 */
            ppid=getppid();            /* 获得父进程的标识符 */
            for(;;) {
                sleep(1);
                kill(ppid,SIGUSR1);
                pause();
            }           
            break;
        default: /*父进程模块*/
            for (;;) {
                pause();
                sleep(1);
                kill(pid,SIGUSR1);
            }
    } 
}

void p_action()
{
    printf("Parent caught signal #%d\n",++ntimes);
}

void c_action()
{
    printf("Child caught signal #%d\n",++ntimes);
}

/*
Parent caught signal #1
Child caught signal #1
Parent caught signal #2
Child caught signal #2
Parent caught signal #3
Child caught signal #3
Parent caught signal #4
Child caught signal #4
^C
*/
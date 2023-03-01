#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

//自定义信号注册函数
static void (*set_signal(int signo,void (*func)(int)))(int)
{
    struct sigaction act,oact;
    act.sa_handler=func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(signo != SIGALRM)
        act.sa_flags|=SA_RESTART;
    if(sigaction(signo,&act,&oact)<0)
        return NULL;
    return(oact.sa_handler);
}

//信号处理函数
static void sig_routine(int signo)
{
    switch(signo)
    {
        case SIGCHLD:
            while(waitpid(-1, NULL, WNOHANG) > 0);
            break;
    }     
    return;
}

void make_daemon()
{
//	set_signal(SIGHUP,SIG_IGN);   0 1 2描述符关闭后还需要吗？
    set_signal(SIGCHLD,sig_routine);
    
    pid_t pid;

    /* 1.创建子进程 */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } 
    else if (pid != 0){   /* parent */
        exit(0);
    } 

    /* 2.创建会话，成为一个新会话的首进程，失去控制终端   */	
    setsid();
    
    /* */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } 
    else if (pid != 0){   /* parent */
        exit(0);
    } 
    
    /* 3.改变当前工作目录到根目录下 */
    if (chdir("/") < 0) {
        perror("chdir");
        exit(1);
    }
    
    /* 4.设置umask为0 */
    umask(0);
    
    /* 5.重定向0，1，2文件描述符到 /dev/null，因为已经失去控制终端，再操作0，1，2没有意义 */
    close(STDIN_FILENO);
    open("/dev/null", O_RDWR);
    dup2(0, STDOUT_FILENO);
    dup2(0, STDERR_FILENO);
    
}
    
int main(void)
{
    make_daemon();
    while(1){
        /* 在此循环中可以实现守护进程的核心工作 */
    }; 
}

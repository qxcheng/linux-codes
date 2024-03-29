#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void donothing(int signo)
{
}

unsigned int mysleep(unsigned int seconds) 
{
    unsigned int ret;  //保存未休眠够的时间

    struct sigaction act, oldact;
    act.sa_handler = donothing;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, &oldact);

    //SIGALRM 阻塞
    alarm(seconds);     //定时固定的秒数  1 
//------失去了CPU-----  2 以后我再次得到CPU
    //SIGALRM 解除阻塞
    pause();         //进程阻塞,收到一个信号后,pause返回-1,解除阻塞

    ret = alarm(0);  //取消旧的定时器,将剩余时间保存
    sigaction(SIGALRM, &oldact, NULL);  //恢复SIGALRM 默认处理方式

    return ret;
}

int main(void)
{
//    mysleep(5);
    sleep(3);
    printf("slept %d s\n", 5);

    return 0;
}

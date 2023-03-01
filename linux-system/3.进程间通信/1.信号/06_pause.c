#include <stdio.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0
#define BELLS "\007\007\007"

int alarm_flag=FALSE;

/* SIGALRM 处理函数 */
setflag()
{
    alarm_flag=TRUE;
}

main(int argc,char* argv[])
{
    int nsecs;
    int i;

    if (argc<2)
    {
        fprintf(stderr,"Usage:tml #minutes message\n");
        exit(1);
    }

    if ((nsecs=atoi(argv[1])*60)<=0)
    {
        fprintf(stderr,"Invalid time\n");
        exit(2);
    }
    
    signal(SIGALRM,setflag);    /* 设定 SIGALRM 的关联动作 */
    alarm(nsecs);               /* 设定定时器 */
    pause();                    /*使用 pause()调用等待信号*/

    if (alarm_flag)
    {
        printf(BELLS);
        for (i=2;i<argc;i++)
        {
            printf("%s\n",argv[i]);
        } 
    }
    exit(0);
}
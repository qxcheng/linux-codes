#include <stdio.h>
#include <signal.h>

#define TIMEOUT 5
#define MAXTRIES 5
#define LINESIZE 100
#define BELL '\007'
#define TRUE 1
#define FALSE 0

/* 判断超时是否已经发生的标志 */
static int time_out;
static char inputline[LINESIZE];
char* quickreply (char* prompt);

main()
{
    printf("%s\n",quickreply("Input"));
}

char* quickreply (char* prompt)
{
    int (*was)(),catch(),ntries;
    char* answer;

    /* 设定捕捉 SIGALRM 的的关联并保存原有关联 */
    was=signal(SIGALRM,catch);

    for (ntries=0;ntries<MAXTRIES;ntries++)
    {
        time_out=FALSE;
        printf("\n%s>",prompt);
        alarm(TIMEOUT);          /* 设定定时器 */
        answer=gets(inputline);  /* 获取输入 */
        alarm(0);                /* 关闭定时器 */
        if (!time_out)
            break;
    }
    /* 恢复原有的 SIGALRM 关联 */
    signal(SIGALRM,was);

    return (time_out?((char*) 0):answer);
}

/* SIGALRM 信号处理函数 */
catch()
{
    time_out=TRUE;  /* 设定超时标志 */
    //putchar(BELL);   /* 响铃警告 */
    //puts("aabb\n");
    sprintf(inputline, "aabb\n");
}
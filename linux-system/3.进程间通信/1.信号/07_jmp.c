#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf position;

main()
{
    int goback();

    /* 保存当前的堆栈环境 */
    setjmp(position);
    signal(SIGINT,goback);
    printf("start\n");
    sleep(5);
    printf("finish\n");

}

goback()
{
    fprintf(stderr,"\nInterrupted\n");
    /* 跳转回被保存的断点 */
    longjmp(position,1);
}
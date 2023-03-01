#include <signal.h>
#include <stdio.h>


int interrupt()
{
    printf("Interrupt called\n");
    sleep(3);
    printf("Interrupt Func Ended.\n");
}

main()
{
    signal(SIGINT,interrupt);
    printf("Interrupt set for SIGINT\n");
    sleep(10);
    printf("Program NORMAL ended.\n");
    return;
}
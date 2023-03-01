#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void catch(int sig);

int main()
{
    void (*oldptr)();
    oldptr = signal(SIGINT, catch);
    if (oldptr == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }
    printf("start\n");
    sleep(10);
    printf("end\n");
    signal(SIGINT, oldptr);
    printf("restore\n");
    sleep(10);
    return;
}

void catch(int sig){
	printf("Catch succeed!\n");
    //exit(1);
	return;
}
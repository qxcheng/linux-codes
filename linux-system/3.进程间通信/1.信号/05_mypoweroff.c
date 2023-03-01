#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define SEC 5

void sig_int(int signo)
{
    system("sudo poweroff");
}

int main(int arg, char *argv[])
{
	if(arg != 2)
	{
		printf("Please offer an argument!\n");
		exit(-1);
	}
	
	struct sigaction act;		

	act.sa_handler = sig_int;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);		 //不屏蔽任何信号
	sigaction(SIGALRM, &act, NULL);

    alarm(atoi(argv[1]));
    
    sleep(atoi(argv[1]));
    printf("-----------over-----------\n");

	return 0;
}

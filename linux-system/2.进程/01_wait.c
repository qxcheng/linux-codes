#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>


int main(void)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	} 
	else if(pid == 0)
	{		//son
		printf("I'm process child, pid = %d\n", getpid());

		execl("./abnor", "abnor", NULL);
		perror("execl error");
		exit(1);

	} 
	else 
	{
		//wpid = wait(NULL);	//传出参数
		wpid = wait(&status);	//传出参数
		if (wpid == -1) {
            perror("wait error");
            exit(1);
        }
			
		if(WIFEXITED(status))  //正常退出
		{	
			printf("I'm parent, The child process \
					%d exit normally\n", wpid);
			printf("return value:%d\n", WEXITSTATUS(status));

		} 
		else if (WIFSIGNALED(status))  //异常退出
		{	
			printf("The child process exit abnormally, \
					killed by signal %d\n", WTERMSIG(status)); //获取信号编号
										
		} 
		else 
		{
			printf("other...\n");
		}
	}

	return 0;
}

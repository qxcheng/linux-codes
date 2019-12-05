#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

// 1.标准示例
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


// waitpid
int main()
{
	int n = 5, i;	
	int status;
	pid_t pid, wpid;
   
	for(i = 0; i < n; i++){
        pid = fork();
		if(pid == 0) {
			break;			
        } 
    }

	if(n == i) // parent
	{  
		sleep(n);
		printf("I am parent, pid = %d\n", getpid());
        for (i = 0; i < n; i++) 
		{
			do {
				wpid = waitpid(0, &status, WNOHANG);				
				if(wpid == 0){
					printf("NO child exited\n");
					sleep(1);		
				}
			} while (wpid == 0);	//子进程未回收
	
			if(wpid > 0){		    //回收了任意子进程
				printf("I'm parent, I catched child process pid = %d\n", wpid);
				if (WIFEXITED(status))
					printf("Child exited with code %d\n", WEXITSTATUS(status));
				else if (WIFSIGNALED(status))
					printf("Child terminated abnormally, signal %d\n", WTERMSIG(status));
			} 
			else {
				printf("error...\n");
			} 
        }
	} 
	else   //child
	{         
		sleep(i);
		printf("I'm %dth child, pid = %d\n", i+1, getpid());
	}
   
	return 0;
}
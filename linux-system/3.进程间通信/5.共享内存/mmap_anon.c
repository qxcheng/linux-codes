#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(void)
{
	int *p;
	pid_t pid;
	
	//注意:只适用于Linux
	p = mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);  //MAP_ANONYMOUS
	if(p == MAP_FAILED){		
		perror("mmap error");
		exit(1);
	}

	pid = fork();			
	if(pid == 0){
		*p = 2000;
		printf("child, *p = %d\n", *p);
	} else {
		sleep(1);
		printf("parent, *p = %d\n", *p);
	}

	munmap(p, 4);				//释放映射区

	return 0;
}

int main(void)
{
	int *p;
	pid_t pid;
	
	int fd;
	fd = open("/dev/zero", O_RDWR); //Unix通用

	p = mmap(NULL, 400, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
	if(p == MAP_FAILED){		
		perror("mmap error");
		exit(1);
	}

	pid = fork();				//创建子进程
	if(pid == 0){
		*p = 2000;
		printf("child, *p = %d\n", *p);
	} else {
		sleep(1);
		printf("parent, *p = %d\n", *p); //为0，因为MAP_PRIVATE
	}

	munmap(p, 4);				//释放映射区

	return 0;
}

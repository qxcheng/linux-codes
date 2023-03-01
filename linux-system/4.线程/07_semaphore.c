// POSIX 标准的无名信号量
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t s;    //定义信号量全局变量

void *tfn(void *arg)
{
	char buf[1024];

	while (1) {
		read(STDIN_FILENO, buf, 1024);
		sem_post(&s);
	}

	return NULL;
}

int main(void)
{
	pthread_t tid;
	struct timespec t = {0, 0};

	sem_init(&s, 0, 0);        //初始化信号量
	
	pthread_create(&tid, NULL, tfn, NULL);
	
	t.tv_sec = time(NULL) + 1; //初始化定时时间
	t.tv_nsec = 0;

	while (1) {
		sem_timedwait(&s, &t); //第一次定时等待1s,以后为5s
		printf("hello world\n");
		t.tv_sec = time(NULL) + 5;
		t.tv_nsec = 0;
	}

	pthread_join(tid, NULL);
	sem_destroy(&s);

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *tfn(void *arg)
{
	while (1) {		
		pthread_testcancel();	//自己添加取消点*/
	}

    pthread_exit((void *)666);
}

int main(void)
{
	pthread_t tid;
	void *tret = NULL;

	pthread_create(&tid, NULL, tfn, NULL);

    pthread_cancel(tid);       //杀死线程
	pthread_join(tid, &tret);  //返回值为 -1
	
	printf("thread 3 exit code = %d\n", (int)tret); 

	return 0;
}


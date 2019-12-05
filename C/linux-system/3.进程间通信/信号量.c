#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>
#include<sys/shm.h>

#if defined( __GNU_LIBRARY__)&&!defined(_SEM_SEMUN_UNDEFINED)
#else
union semun{
	int val;/*Value for SETVAL*/
	struct semid_ds *bur; /*Buffer for IPC_STAT,IPC_SET*/
	unsigned short *array; /*Array for GETALL,SETALL*/
	struct seminfo *__but; /*Buffer for IPC_INFO(Linux specific)*/
};
#endif

int semheld = 0;
int id = 0;

void pid_printf(char *format,...)
{
	va_list ap;
	va_start(ap,format);
	printf("[%d]:",getpid());
	vprintf(format,ap);
}

//释放信号量
void sem_release(int id)
{
	struct sembuf sb;
	if(semheld<1){
		pid_printf("I don't have any reources; nothing to release\n");
		return;
	}
	sb.sem_num=0;
	sb.sem_op=1;
	sb.sem_flg = SEM_UNDO;
	if(semop(id,&sb,1) == -1){
		pid_printf("Semop release error: %s\n",strerror(errno));
		exit(-1);
	}
	semheld--;
	pid_printf("Resource released.\n");
}

//请求信号量
void sem_request(int id)
{
	struct sembuf sb;
	if(semheld>0){
		pid_printf("I already hold the resource; not requesting another
		one.\n");
		return;
	}
	sb.sem_num =0;
	sb.sem_op = -1;
	sb.sem_flg=SEM_UNDO;
	pid_printf("Requesting resource ...");
	fflush(stdout);
	if(semop(id,&sb,1)==-1){
		pid_printf("Semop request error: %s\n",strerror(errno));
		exit(-1);
	}
	semheld++;
	printf("Done.\n");
}

/*删除信号量集*/
void sem_delete(void)
{
	printf("Master exiting; delete semaphore.\n");
	if(semctl(id,0,IPC_RMID,0)== -1){
		pid_printf("Error releasing semaphone.\n");
	}
}

int main(int argc, char **argv)
{
	union semun sunion;
	if(argc<2){
		id = semget(IPC_PRIVATE,1,SHM_R | SHM_W);
		if(id !=-1){
			atexit(&sem_delete);
			sunion.val=1;
			if(semctl(id,0,SETVAL,sunion) ==1){
				pid_printf("semctl failed: %s\n",strerror(errno));
				exit(-1);
			}
		}
	}
	else{
		id = atoi(argv[1]);
		pid_printf("Using existing semaphore %d.\n",id);
	}
	if(id==-1){
		pid_printf("Semaphore request failed: %s.\n", strerror(errno));
		return 0;
	}
	pid_printf("Successfully allocated semaphore id %d.\n",id);
	while(1){
		int action;
		printf("\nStatus: %d request held by this process.\n", semheld);
		printf("Please select:\n");
		printf("1. Release a resource\n");
		printf("2. Request a resource\n");
		printf("3. Exit this process\n");
		printf("Your choice:");scanf("%d",&action);
		switch(action){
			case 1:
				sem_release(id);
				break;
			case 2:
				sem_request(id);
				break;
			case 3:
				exit(0);
				break;
		}
	}
	return 0;
}
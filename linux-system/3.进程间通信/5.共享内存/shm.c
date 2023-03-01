#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main(int argc, char *argv[])
{
	int shmid;
	char *segptr;
    char buff[100];

	key_t key = ftok(".", 'S');
	/* 创建共享内存，已存在返回-1 */
	if((shmid = shmget(key, 100, IPC_CREAT|IPC_EXCL|0660)) == -1) 
    {
		printf("Shared memory segment exists - opening as client\n");
	    /* 已存在尝试获取 */
		if((shmid = shmget(key, 100, 0)) == -1) {
			perror("shmget");
			exit(1);
		}
	} 
    //创建成功 
	else 
    {
		printf("Creating new shared memory segment id = %d\n ", shmid);
	}
	
	/* 连接共享内存 */
	if( (segptr = (char *)shmat(shmid, NULL, 0)) == (void * )-1) {
		perror("shmat");
		exit(1);
	}

    pid_t pid = fork();
    if( -1 == pid){
        perror("fork");
        exit(0);
    }
	else if(pid > 0){                    // 后续待添加互斥机制
        while(1){
            scanf("%s",buff);
            strcpy(segptr, buff);
        }  
    }
    else{
        while(1){
            sleep(2);
            printf("segptr: %s\n", segptr);
        }
        
    }
}
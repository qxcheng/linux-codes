#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

// Message struct
struct mymsgbuf {
    long mtype;    /* Message type */
    //int request;   /* Work request number */
    char mtext[1024]; /* Employee's salary */
};

// create message queue
int open_queue( key_t keyval )
{
	int qid;

	if((qid = msgget( keyval, IPC_CREAT | 0660 )) == -1)
	{
		return(-1);
	}

	return(qid);
}

int send_message( int qid, struct mymsgbuf *qbuf )
{
	int result, length;
	/* The length is essentially the size of the structure minus sizeof(mtype) */
	length = sizeof(struct mymsgbuf) - sizeof(long);
	if((result = msgsnd( qid, qbuf, length, 0)) == -1)
	{
		return(-1);
	}
	return(result);
}

int read_message( int qid, long type, struct mymsgbuf *qbuf )
{
	int result, length;
	/* The length is essentially the size of the structure minus sizeof(mtype) */
	length = sizeof(struct mymsgbuf) - sizeof(long);
	if((result = msgrcv( qid, qbuf, length, type, 0)) == -1)
	{
		return(-1);
	}
	return(result);
}

// checking if exists the "type" of message
int peek_message( int qid, long type )
{
	int result, length;
	if((result = msgrcv( qid, NULL, 0, type, IPC_NOWAIT)) == -1)
	{
		if(errno == E2BIG)
			return(TRUE);
		}
	return(FALSE);
}


int main(int argc, char *argv[])
{
    int qid;
    key_t msgkey;
    struct mymsgbuf wmsg, rmsg;
    char text[1024] = {'\0'};

    /* Generate our IPC key value */
    msgkey = ftok(".", 'm');

    /* Open/create the queue */
    if(( qid = open_queue( msgkey)) == -1) {
        perror("open_queue");
        exit(1);
    }
    printf("Creating message queue id = [%d]\n", qid);

    pid_t pid = fork();
    if(pid == -1){
        perror("fork error");
        exit(1);
    }
    else if(pid > 0){
        wmsg.mtype = 1;
        while(1){
            sleep(1);
            printf("Please input your message:\n");
            scanf("%s", text);
            strcpy(wmsg.mtext, text);
            if(send_message(qid, &wmsg) == -1){
                perror("msgsend");
                exit(1);
            }
        }
    }
    else{
        while(1){
            rmsg.mtype = 1;
            if(read_message(qid, rmsg.mtype, &rmsg) == -1){
                perror("msgread");
                exit(1);
            }
            printf("Type: %ld\nText: %s\n", rmsg.mtype, rmsg.mtext);
        }
    }
}
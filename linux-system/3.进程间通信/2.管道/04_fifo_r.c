#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(){

    char buf[1024];

    //读端
    int ret = mkfifo("fifo", 0660);  
    if(ret < 0){ 
        perror("mkfifo");
        exit(-1);
    }	

    int fd = open("fifo", O_RDONLY);
    if (fd < 0){ 
        perror("open");
        exit(1); 
    }	

    while(1){
        int num = read(fd, buf, sizeof(buf));   
        if(0 == num){ 
            break; 
        }
        buf[num] = '\0';
        printf("read buf = %s\n",buf);
    }
    close(fd);

}



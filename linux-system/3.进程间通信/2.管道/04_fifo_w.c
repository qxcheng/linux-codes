#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(){

    char buf[1024];

    //写端
    
    int fd = open("fifo", O_WRONLY);
    if (fd < 0){ 
        perror("open");
        exit(1); 
    }	

    while(1){
        scanf("%s", buf);
        int num = write(fd, buf, sizeof(buf));   
    }
    close(fd);

}
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, char* argv[])
{
    printf("myprocess begin\n");
    char buf[30];
    bzero(buf, sizeof(buf));

    read(STDIN_FILENO, buf, sizeof(buf));

    printf("recv message:%s\n", buf);
    
    exit(33);
    
}
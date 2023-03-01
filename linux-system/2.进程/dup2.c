#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main()
{
    int fd;

    fd = open("data", O_RDWR);;
    if(fd<0) {
        perror("open");
        exit(1);
    }
	
	//dup2(fd, STDOUT_FILENO);        //标准输出重定向到fd，ps执行结果写入文件
	//close(fd);
	//execlp("ps", "ps","aux", NULL);
	
    dup2(fd, STDIN_FILENO);           //标准输入重定向到fd，即从fd获取输入	
    close(fd);	
    execl("./upper", "upper", NULL);
   
    return 0;
}

 /* upper
#include <ctype.h>
#include <stdio.h>
int main(void)
{
    int ch;
    while((ch = getchar()) != EOF) {
        putchar(toupper(ch));
    }
    return 0;
}	
*/	
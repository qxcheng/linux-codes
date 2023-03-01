#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char **argv){

    int fd;

    fd = open(argv[1], O_WRONLY);
    // 若文件存在
    if (fd != -1){
        printf("[PID %ld] File \"%s\" already exists\n", (long)getpid(), argv[1]);
        close(fd);
    // 若文件不存在
    } else {
        // 发生了其他未知错误（文件可能存在）
        if (errno != ENOENT){
            perror("open1");
        } else {
            // 走到这里初步认为文件确实不存在，然后创建文件
            fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1) 
                perror("open2");
            printf("[PID %ld] Created file \"%s\" exclusively\n", (long)getpid(), argv[1]);
        }
    }
}
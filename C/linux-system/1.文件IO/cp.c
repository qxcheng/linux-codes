#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int write_buffer(int fd, const void *buf, size_t count ) 
{
    int n = 0, r = count;
    while( count > 0 ) {
        n = write( fd, buf, count );
        if( n == 0 ) {
            r = 0; break;
        }
        if( n > 0 ) {
            count -= n;
            buf += n;
        } 
        else if( n < 0 ) {
            /*由于中断信号造成的写函数返回，重新写入*/
            if( errno == EINTR )
                continue;
            r = -1;
            break;
        }
    }
    return r;
}

int main( int argc, char **argv )
{
    int fd_read;
    int fd_write;
    char buf[128]={0};
    int bytes_read, bytes_write;
    /*判断是否传入文件名*/
    if( argc < 3 ){
        printf("Usage: write_test FILEREAD FILEWRITE\n");
        return -1;
    }
    /*打开要读取的文件*/
    fd_read = open( argv[1], O_RDONLY );
    if( fd_read == -1 ){
        perror( "open error" );
        return -2;
    }
    /*创建一个新文件*/
    fd_write = open( argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0640 );
    if( fd_write == -1 ){
        perror( "create error" );
        close( fd_read );
        return -3;
    }
    /*循环读取数据，并把每次读到的数据写入文件*/
    while( bytes_read = read( fd_read, buf, sizeof( buf )-1 ) ) {
        if( bytes_read > 0 ) {
            bytes_write = write_buffer( fd_write, buf, bytes_read );
            memset(buf,0,sizeof(buf));//请清空缓冲区
            /*write时候发生错误*/
            if( bytes_write == -1 ) break;
        }
        else if( ( bytes_read == -1 ) &&( errno != EINTR ) ){
            /*read错误发生了*/
            break;
        }
    }
    close( fd_read );
    close( fd_write );
    return 0;
}

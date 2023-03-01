#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main( int argc, char **argv )
{
	struct stat sbuf;
	char buf[128];
	int ret;
	
	/*判断是否传入文件名*/
	if( argc != 2 ){
		printf("Usage: stat_test FILENAME\n");
		return -1;
	}
	printf("Information for file[%s]:\n", argv[1]);
	
	/*用 lstat 取文件属性*/
	if( lstat( argv[1], &sbuf ) ){
		perror( "lstat" );
		return -2;
	}
	
	/*判断是否是链接文件*/
	if( S_ISLNK( sbuf.st_mode ) ){
		printf("This is a symbolic link." );
		
		if( readlink( argv[1], buf, sizeof( buf ) ) == -1 ){
			printf(" Can't read link!\n");
			return -3;
		}
		printf(" Link to file [%s]\n", buf);
		
		/*如果是符号链接文件，用 stat 取被链接的文件的属性*/
		if( stat( argv[1], &sbuf ) ){
			perror( "stat" );
			return -4;
		}
		printf("Below are information for file pointed to by link:\n");
	}
	
	/*显示文件属性*/
	printf( "%30s:\t%d\n", "ID of device containing file", sbuf.st_dev);
	printf( "%30s:\t%d\n", "inode number", sbuf.st_ino);
	printf( "%30s:\t%d\n", "Number of hard links", sbuf.st_nlink);
	printf( "%30s:\t", "File type");
	
	if( S_ISREG( sbuf.st_mode ) ){
		printf("Regular file");
	}
	else if( S_ISDIR( sbuf.st_mode ) ){
		printf("Directory");
	}
	else if( S_ISCHR( sbuf.st_mode ) ){
		printf("Character device");
	}
	else if( S_ISBLK( sbuf.st_mode ) ){
		printf("Block device");
	}
	else if( S_ISFIFO( sbuf.st_mode ) ){
		printf("FIFO");
	}
	else if( S_ISSOCK( sbuf.st_mode ) ){
		printf("Socket");
	}
	
	printf("\n");
	printf( "%30s:\t%d\n", "Device ID", sbuf.st_rdev);
	printf( "%30s:\t%d\n", "File sizes", sbuf.st_size);
	printf( "%30s:\t%d\n", "Perfered block size", sbuf.st_blksize);
	printf( "%30s:\t%s", "time of last access", ctime( &sbuf.st_atime ));
	printf( "%30s:\t%s", "time of last modification", ctime( &sbuf.st_mtime ));
	printf( "%30s:\t%s", "time of last status change", ctime( &sbuf.st_ctime ));
	
	return 0;
}
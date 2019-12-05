#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main( int argc, char **argv )
{
	struct stat sbuf;
	mode_t mode, chm;
	int i;
	
	/*判断是否传入文件名*/
	if( argc != 3 ){
		printf("Usage: stat_test [+-=][rwx] FILENAME\n");
		return -1;
	}
	
	/*分析参数*/
	if( argv[1][0] != '+' && argv[1][0] != '-' ){
		printf("Wrong parameter!\n");
		return -2;
	}
	
	chm = 0;
	
	for( i = 1; i < strlen( argv[1] ); i++ ){
		if( argv[1][i] == 'r' ){
			chm |= ( S_IRUSR | S_IRGRP | S_IROTH );
		}
		else if( argv[1][i] == 'w' ){
			chm |= ( S_IWUSR | S_IWGRP | S_IWOTH );
		}
		else if( argv[1][i] == 'x' ){
			chm |= ( S_IXUSR | S_IXGRP | S_IXOTH );
		}
		else{
			printf("Wrong parameter!\n");
			return -2;
		}
	}
	
	/*用stat取文件属性*/
	if( stat( argv[2], &sbuf ) ){
		perror( "stat" );
		return -3;
	}
	mode = sbuf.st_mode & 0777;
	printf("Change mode of[%s] from [%o]", argv[1], mode);
	
	/*修改权限状态*/
	if( argv[1][0] == '+' ){
		mode |= chm;
	}
	else if( argv[1][0] == '-' ){
		mode &= ~chm;
	}
	printf(" to [%o]\n", mode);
	
	/*修改文件权限*/
	if( chmod( argv[2], mode ) ){
		perror( "chmod()" );
	}
	
	return 0;
	
}
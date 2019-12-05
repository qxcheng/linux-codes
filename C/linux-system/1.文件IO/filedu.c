#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/*记录文件长度*/
static long length = 0;

/*过滤文件名为.和..的文件*/
int myfilter(const struct dirent *dr)
{
	if( strcmp( dr->d_name, "." ) == 0 ){
		return 0;
	}else if( strcmp( dr->d_name, ".." ) == 0 ){
		return 0;
	}
	return 1;
}

int mydu( const char *path )
{
	struct dirent **namelist;
	int n, i;
	struct stat sbuf;
	n = scandir( path, &namelist, myfilter, alphasort);
	if (n < 0){
		perror("scandir");
	} else {
		for( i = 0; i < n; i ++ ){
			/*用 stat 查看文件属性*/
			if( lstat( namelist[i]->d_name, &sbuf )){
				perror( "lstat" );
				continue;
			}
			/*累加文件长度*/
			length += sbuf.st_size;
			/*判断是否是目录*/
			if( S_ISDIR( sbuf.st_mode ) ){
				mydu( namelist[i]->d_name );
			}
			/*释放内存*/
			free(namelist[i]);
		}
		free(namelist);
	}
}

int main( int argc, char **argv )
{
	if( argc == 1 ){
		/*没有传入目录名，统计当前目录*/
		mydu( "." );
	}else if( argc == 2 ){
		/*传入了目录名，统计当前目录*/
		mydu( argv[1] );
	}else{
		printf("Usage: mydu [DIRNAME]\n");
		return -1;
	}
	printf("Totol size is [%ld]\n", length);
	return 0;
}
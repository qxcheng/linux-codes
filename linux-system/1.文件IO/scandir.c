#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int myfilter(const struct dirent *dr)
{
	/*过滤所有文件名以.开头的文件*/
	if( strcspn( dr->d_name, "." ) == 0 ){ 
		return 0;
	}
	//过滤目录
	else if(dr->d_type == DT_DIR){
		return 0;
	}
	return 1;
}

int main( int argc, char **argv )
{
	struct dirent **namelist;
	int n;

	/*没有传入目录名，列出当前目录的文件*/
	if( argc == 1 )
	{	
		n = scandir(".", &namelist, myfilter, alphasort);
	}
	/*传入了目录名，列出指定目录的文件*/
	else if( argc == 2 )
	{
		n = scandir(argv[1], &namelist, myfilter, alphasort);
	}
	else
	{
		printf("Usage: scandir_test [DIRNAME]\n");
		return -1;
	}

	//错误处理
	if (n < 0)
	{
		perror("scandir");
	} 
	//打印输出
	else
	{
		for( int i = 0; i < n; i++ )
		{
			printf("%s\n", namelist[i]->d_name);
			free(namelist[i]);
		}
		free(namelist);
	}

	return 0;
}
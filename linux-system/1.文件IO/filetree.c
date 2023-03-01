#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

static int depth = 0;

void print_filename(char *name, int last)
{
	int i;
	for(i = 1; i < depth; i++ ){
		printf("\t");
	}
	if(last){
		printf("`-- %s\n", name);
	}else{
		printf("|-- %s\n", name);
	}
}

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

int mytree( const char *path )
{
	struct dirent **namelist;
	int n, i;
	struct stat sbuf;
	char realpath[1024]={0};
	/*目录深度加 1*/
	depth++;
	n = scandir( path, &namelist, myfilter, alphasort);
	if (n < 0){
		perror("scandir");
	} else {
		for( i = 0; i < n; i ++ ){
			print_filename(namelist[i]->d_name, ( i == ( n - 1 ) ) );
			/*用 stat 查看文件属性*/
			strcpy(realpath, path);
			strcat(realpath, "/");
			strcat(realpath, namelist[i]->d_name);
			if( lstat(realpath, &sbuf ) ){
				perror( "lstat" );
				return -2;
			}
			/*判断是否是目录*/
			if( S_ISDIR( sbuf.st_mode ) ){
				mytree(realpath);
			}
			/*释放内存*/
			free(namelist[i]);
		}
		free(namelist);
	}
	/*目录深度减 1*/
	depth--;
}

int main( int argc, char **argv )
{
	if( argc == 1 ){
		/*没有传入目录名，列出当前目录树*/
		return mytree( "." );
	}else if( argc == 2 ){
		/*传入了目录名，列出指定目录树*/
		return mytree(argv[1]);
	}else{
		printf("Usage: mytree [DIRNAME]\n");
		return -1;
	}
	return 0;
}
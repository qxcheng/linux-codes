/* 大端格式按地址从低位到高位存放数据 */

#include<stdio.h>

int main()
{

#if 0
	
	union person
    {
    	int num;
    	char name[4];
    }p1;
    strcpy(p1.name, "ABCD");  //即0x41424344
    printf("0x%x\n", p1.num);       
	
#else
	
	int num = 0x41424344;
    int *p = &num;
    printf("0x%x\n", *(char *)p);  
	
#endif

	return 0;
	
}
   
   
    

   


//char *strstr(const char *str1, const char *str2)
//返回str1中第一个str2的起始位置的指针

#include<stdio.h>
#include<string.h>

void strstr1(char *str1, char *str2)
{
    int length = strlen(str2);  
    char *s1=str1, *s2=str2;
    
    int flag=1;

    while(*(s1+length-1) != '\0')
    {
        for(int i=0; i<length; i++)
        {
            if(*(s1+i) != *(s2+i))
                flag = 0;
        }
        if(flag)
        {
           printf("%c, ", *s1); 
        }
        flag = 1;
        s1++;
    }
}

int main()
{
    char *str1="abc123abc";
    char *str2="abc";
    strstr1(str1, str2);
    return 0;
}

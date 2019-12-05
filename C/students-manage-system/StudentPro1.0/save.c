#include <stdio.h>
#include "save.h"
#include "student.h"
#include "teacher.h"
#include "common.h"

/*
 * 写入学生信息
 */
void write_stu(int len)
{
    FILE *fp;
    fp = fopen("student.txt", "w");
    fwrite(STU, sizeof(student), len, fp);
    fclose(fp);
}

//读取学生信息到结构体数组
int read_stu()
{
    FILE *fp;
    fp = fopen("student.txt", "r");
    int len=0;
    while(fread(&STU[len], sizeof(student), 1, fp) > 0)
    {
        len++;     
    }
    fclose(fp);
    return len;
}

//写入老师信息
void write_tea(int len)
{
    FILE *fp;
    fp = fopen("teacher.txt", "w");
    fwrite(TEA, sizeof(struct teacher), len, fp);
    fclose(fp);
}

//读取老师信息
int read_tea()
{
    FILE *fp;
    fp = fopen("teacher.txt", "r");
    int len=0;
    while(fread(&TEA[len], sizeof(struct teacher), 1, fp) > 0)
    {
        len++;     
    }
    fclose(fp);
    return len;
}

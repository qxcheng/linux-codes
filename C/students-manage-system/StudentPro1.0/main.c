#include <stdio.h>
#include "menu.h"
#include "student.h"
#include "teacher.h"
#include "save.h"
#include "common.h"

int len_stu;             //学生人数
int len_tea;             //老师人数
struct student STU[100]; //存放学生的结构体
struct teacher TEA[100]; //存放老师的结构体

int main()
{
    len_stu = read_stu();
    len_tea = read_tea();
    
    menu();
    return 0;
}

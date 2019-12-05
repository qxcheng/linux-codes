#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "save.h"
#include "menu.h"
#include "common.h"

//学生界面菜单
void stu_menu(int i)
{
    system("clear");

    printf("********欢迎 %3s 登录*********\n", STU[i].name);
    printf("*                            *\n");
    printf("*      1 查看个人信息        *\n");
    printf("*                            *\n");
    printf("*      2 修改密码            *\n");
    printf("*                            *\n");
    printf("*      3 返回登录界面        *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("******************************\n");

    int key;
    printf("请选择您的操作:\n");
    scanf("%d", &key);

    char pass1[32];

    switch(key)
    {
        case 1:
            printf("姓名: %s\n", STU[i].name);
            printf("学号: %d\n", STU[i].id);
            printf("年龄: %d\n", STU[i].age);
            printf("班级: %d\n", STU[i]._class);
            printf("数学成绩: %d\n", STU[i].math);
            printf("C语言成绩: %d\n", STU[i].c);
            printf("语文成绩: %d\n", STU[i].chinese);
            printf("年级名次: %d\n", STU[i].no);
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            stu_menu(i); 
            break;
        case 2:
            printf("请输入旧密码:\n");
            scanf("%s", pass1);
            if(strcmp(pass1, STU[i].passwd)==0)
            {
                printf("请输入新密码:\n");
                scanf("%s", STU[i].passwd);
                write_stu(len_stu);
                printf("修改成功!\n");
                printf("请按任意键继续!\n");
                getchar();
                getchar();
                stu_menu(i);
            }
            else
            {
                printf("密码错误，请重新输入!\n");
                printf("请按任意键继续!\n");
                getchar();
                getchar();
                stu_menu(i);
            }
            break;
        case 3:
            menu();
            break;
        default :
            printf("您的输入有误，请重新输入！\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            stu_menu(i);
    }
}

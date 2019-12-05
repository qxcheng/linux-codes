#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "save.h"
#include "student.h"
#include "teacher.h"
#include "common.h"
#include "admin.h"

/*
 * main menu
 */
void menu()
{
    system("clear");

    printf("*********学生管理系统*********\n");
    printf("*                            *\n");
    printf("*        1 学生登录          *\n");
    printf("*                            *\n");
    printf("*        2 老师登录          *\n");
    printf("*                            *\n");
    printf("*        3 学生注册          *\n");
    printf("*                            *\n");
    printf("*        4 老师注册          *\n");
    printf("*                            *\n");
    printf("*        5 管理员登录        *\n");
    printf("*                            *\n");
    printf("*        6 退出系统          *\n");
    printf("*                            *\n");
    printf("******************************\n");

    printf("\n请选择您的操作:\n");
    int key;
    scanf("%d", &key);

    int id1, id2;
    char passwd1[32], passwd2[32];
    int i, j;

    switch(key)
    {
        case 1:
            printf("*******学生用户登录**********\n\n");
            printf("请输入用户名:\n");
            scanf("%d", &id1);
            printf("请输入密码:\n");
            scanf("%s", passwd1);
            for(i=0; i<len_stu; i++)
            {
                if(STU[i].id==id1 && strcmp(STU[i].passwd,passwd1)==0)
                {
                    stu_menu(i);
                    break;
                }
            }
            printf("用户名户或密码错误,请重新输入\n");
            printf("请输入任意键继续\n");
            getchar();
            getchar();
            menu();
            break;
        case 2:
            printf("*******老师用户登录*********\n\n");
            printf("请输入用户名:\n");
            scanf("%d", &id2);
            printf("请输入密码:\n");
            scanf("%s", passwd2);
            for(j=0; j<len_tea; j++)
            {
                if(TEA[j].id==id2 && strcmp(TEA[j].passwd,passwd2)==0)
                {
                    tea_menu(j);
                    break;
                }
            }
            printf("用户名户或密码错误,请重新输入\n");
            printf("请输入任意键继续\n");
            getchar();
            getchar();
            menu();
            break;
        case 3:
            printf("*******欢迎新同学注册*******\n");
            if(len_stu == 0)
            {
                STU[len_stu].id = 1001;
            }
            else
            {
                STU[len_stu].id = STU[len_stu - 1].id + 1;     
            }
            printf("系统为您分配的学号为:%d\n", STU[len_stu].id);
            printf("请输入您的姓名:\n");
            scanf("%s", STU[len_stu].name);
            printf("请输入您的年龄:\n");
            scanf("%d", &STU[len_stu].age);
            printf("请输入您的班级:\n");
            scanf("%d", &STU[len_stu]._class);
            printf("请输入您的数学成绩:\n");
            scanf("%d", &STU[len_stu].math);
            printf("请输入您的C语言成绩:\n");
            scanf("%d", &STU[len_stu].c);
            printf("请输入您的语文成绩:\n");
            scanf("%d", &STU[len_stu].chinese);
            printf("请输入您的密码:\n");
            scanf("%s", STU[len_stu].passwd);
            if(len_stu == 0)
            {
                STU[len_stu].no = 1;     
            }
            else
            {
                STU[len_stu].no = len_stu + 1;
                int sum_last = STU[len_stu-1].math + STU[len_stu-1].c + STU[len_stu-1].chinese;     
                int sum_cur = STU[len_stu].math + STU[len_stu].c + STU[len_stu].chinese; 
                int n=len_stu-1;
                while(sum_cur > sum_last)
                {
                    STU[len_stu].no--;
                    STU[n].no++;
                    n--;
                    if(n>=0)
                    {
                        sum_last = STU[n].math + STU[n].c + STU[n].chinese; 
                    }
                    else
                    {
                        break;     
                    }
                }
                
            }
            len_stu++;
            write_stu(len_stu);
            printf("注册成功\n");
            printf("请输入任意键继续\n");
            getchar();
            getchar();
            menu();
            break;
        case 4:
            printf("******欢迎新老师注册******\n");
            if(len_tea == 0)
            {
                TEA[len_tea].id = 2001;     
            }
            else
            {
                TEA[len_tea].id = TEA[len_tea-1].id + 1;     
            }         
            printf("系统为您分配的用户名为:%d\n", TEA[len_tea].id);
            printf("请输入您的姓名:\n");
            scanf("%s", TEA[len_tea].name);
            printf("请输入您所在的班级:\n");
            scanf("%d", &TEA[len_tea]._class);
            printf("请输入您的密码:\n");
            scanf("%s", TEA[len_tea].passwd);
            len_tea++;
            write_tea(len_tea);
            printf("注册成功\n");
            printf("请输入任意键继续\n");
            getchar();
            getchar();
            menu();
            break;
        case 5:
            id2 = 123456;
            strcpy(passwd2, "123456");
            printf("请输入管理员帐号:\n");
            scanf("%d", &id1);
            printf("请输入管理员密码:\n");
            scanf("%s", passwd1);
            if(id2 == id1 && strcmp(passwd2,passwd1)==0)
            {
                admin_menu();
                printf("欢迎管理员登录!\n");
            }
            break;
        case 6:
            exit(0);
            break;
            
    }
}

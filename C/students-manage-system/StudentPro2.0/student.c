#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "student.h"
#include "save.h"
#include "menu.h"


//学生界面菜单
void stu_menu(int id, char *passwd)
{
    system("clear");

    printf("**********欢迎登录************\n");
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

    char sql[100];
    char passwd1[32];

    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        

    switch(key)
    {
        case 1:
            sprintf(sql,"select * from STU where id=%d",id);
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            stu_menu(id,passwd); 
            break;
        case 2:
            printf("请输入旧密码:\n");
            scanf("%s", passwd1);
            if(strcmp(passwd1, passwd)==0)
            {
                printf("请输入新密码:\n");
                scanf("%s", passwd1);
                sprintf(sql,"update STU set passwd='%s' where id=%d",passwd1,id);
                save_data(sql);
                mysql_close(&mysql);
                getchar();
                getchar();
                stu_menu(id,passwd1);
            }
            else
            {
                printf("密码错误，请重新输入!\n");
                mysql_close(&mysql);
                getchar();
                getchar();
                stu_menu(id,passwd);
            }
            break;
        case 3:
            mysql_close(&mysql);
            menu();
            break;
        default :
            printf("您的输入有误，请重新输入！\n");
            mysql_close(&mysql);
            getchar();
            getchar();
            stu_menu(id,passwd);
    }
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "menu.h"
#include "save.h"
#include "student.h"
#include "teacher.h"
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

    int id;
    char passwd[32];

    switch(key)
    {
        case 1:
            login_stu();
            break;
        case 2:
            login_tea();
            break;
        case 3:
            signup_stu();
            break;
        case 4:
            signup_tea();
            break;
        case 5:
            printf("请输入管理员帐号:\n");
            scanf("%d", &id);
            printf("请输入管理员密码:\n");
            scanf("%s", passwd);
            if(123456 == id && strcmp(passwd,"123456")==0)
                admin_menu();
            else
            {
                printf("您尚未拥有管理员权限\n");
                menu();
            }
            break;
        case 6:
            exit(0);
            break;
    }
}

void login_stu()
{
    int id;
    char passwd[32];
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    printf("*******学生用户登录**********\n\n");
    printf("请输入用户名:\n");
    scanf("%d", &id);
    printf("请输入密码:\n");
    scanf("%s", passwd);

    char sql[100];
    sprintf(sql,"select * from STU where id=%d and passwd='%s'", id, passwd);
    mysql_real_query(&mysql,sql,strlen(sql));
    MYSQL_RES *result = mysql_store_result(&mysql);
    int num = mysql_num_rows(result);
    if(num == 1)
    {   
        stu_menu(id,passwd);
        mysql_close(&mysql);
    }
    else
    {
        printf("用户名户或密码错误!\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        menu();
        return ;
    }
}

void login_tea()
{
    int id;
    char passwd[32];
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))
        printf("database connect fail!\n");        
    printf("*******老师用户登录**********\n\n");
    printf("请输入用户名:\n");
    scanf("%d", &id);
    printf("请输入密码:\n");
    scanf("%s", passwd);

    char sql[100];
    sprintf(sql,"select * from TEA where id=%d and passwd='%s'", id, passwd);
    if(mysql_real_query(&mysql,sql,strlen(sql)) == 0);
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        int num = mysql_num_rows(result);
        if(num == 1)
        {  
            tea_menu(id,passwd);
            mysql_close(&mysql);
        }
        else
        {
            printf("用户名户或密码错误!\n");
            mysql_close(&mysql);
            getchar();
            getchar();
            menu();
            return ;
        }
    }
}

void signup_stu()
{
    int age,_class,math,c,chinese;
    char name[128],passwd[32];
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    printf("*******欢迎新同学注册*******\n");
    printf("请输入您的姓名:\n");
    scanf("%s", name);
    printf("请输入您的年龄:\n");
    scanf("%d", &age);
    printf("请输入您的班级:\n");
    scanf("%d", &_class);
    printf("请输入您的数学成绩:\n");
    scanf("%d", &math);
    printf("请输入您的C语言成绩:\n");
    scanf("%d", &c);
    printf("请输入您的语文成绩:\n");
    scanf("%d", &chinese);
    printf("请输入您的密码:\n");
    scanf("%s", passwd);

    char sql[100];
    sprintf(sql,"insert into STU (name,passwd,age,_class,math,c,chinese) values('%s','%s', %d, %d, %d, %d, %d)",name,passwd,age,_class,math,c,chinese);
    if(mysql_real_query(&mysql,sql,strlen(sql)) == 0)
    {
        printf("注册成功\n");
        no_sort();
        mysql_close(&mysql);
        getchar();
        getchar();
        menu();
    }
    else
    {
        printf("注册失败\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        menu();
    }
}

void signup_tea()
{
    int _class;
    char name[128],passwd[32];
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    printf("*******欢迎新老师注册*******\n");
    printf("请输入您的姓名:\n");
    scanf("%s", name);
    printf("请输入您的班级:\n");
    scanf("%d", &_class);
    printf("请输入您的密码:\n");
    scanf("%s", passwd);

    char sql[100];
    sprintf(sql,"insert into TEA (name,passwd,_class) values('%s','%s', %d)",name,passwd,_class);
    if(mysql_real_query(&mysql,sql,strlen(sql)) == 0)
    {
        printf("注册成功\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        menu();
    }
    else
    {
        printf("注册失败\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        menu();
    }
}

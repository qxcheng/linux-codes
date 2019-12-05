#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "menu.h"
#include "student.h"
#include "teacher.h"
#include "save.h"
#include "admin.h"

//老师界面菜单
void tea_menu(int id, char* passwd)
{
    system("clear");

    printf("*********欢迎老师登录*********\n");
    printf("*                            *\n");
    printf("*       1 查看基本信息       *\n");
    printf("*                            *\n");
    printf("*       2 修改密码           *\n");
    printf("*                            *\n");
    printf("*       3 显示学生信息       *\n");
    printf("*                            *\n");
    printf("*       4 查找学生信息       *\n");
    printf("*                            *\n");
    printf("*       5 修改学生成绩       *\n");
    printf("*                            *\n");
    printf("*       6 返回登录界面       *\n");
    printf("*                            *\n");
    printf("******************************\n");
    printf("请选择您的操作:\n");

    char passwd1[32];
    int id1;

    char sql[100];
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        

    int key;
    scanf("%d", &key);
    switch(key)
    {
        case 1:
            sprintf(sql,"select * from TEA where id=%d",id);
            printf("ID\t姓名\t密码\t班级\n");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            tea_menu(id,passwd);
            break;
        case 2:
            printf("请输入原密码:\n");
            scanf("%s", passwd1);
            if(strcmp(passwd1, passwd)==0)
            {
                printf("请输入新密码:\n");
                scanf("%s", passwd1);
                sprintf(sql, "update TEA set passwd='%s' where id=%d", passwd1,id);
                save_data(sql);
                mysql_close(&mysql);
                getchar();
                getchar();
                tea_menu(id,passwd1);
            }
            else
            {
                printf("原密码错误,请重新输入!\n");
                mysql_close(&mysql);
                getchar();
                getchar();
                tea_menu(id,passwd);
            }
            break;
        case 3:
            mysql_close(&mysql);
            show_stu(id,passwd);
            break;
        case 4:
            mysql_close(&mysql);
            find_stu(id,passwd);
            break;
        case 5:
            mysql_close(&mysql);
            set_stu(id,passwd);
            break;
        case 6:
            mysql_close(&mysql);
            menu();
            break;
        default :
            mysql_close(&mysql);
            printf("您的输入有误，请重新输入！\n");
            getchar();
            getchar();
            tea_menu(id,passwd);
    }

}

//显示学生信息
void show_stu(int id, char* passwd)/*{{{*/
{
    system("clear");

    printf("******************************\n");
    printf("*                            *\n");
    printf("*     1 按学号显示           *\n");
    printf("*                            *\n");
    printf("*     2 按名次显示           *\n");
    printf("*                            *\n");
    printf("*     3 按数学成绩显示       *\n");
    printf("*                            *\n");
    printf("*     4 按C语言成绩显示      *\n");
    printf("*                            *\n");
    printf("*     5 按语文成绩显示       *\n");
    printf("*                            *\n");
    printf("*     6 返回上级菜单         *\n");
    printf("*                            *\n");
    printf("******************************\n");

    char sql[100];
    char sql1[100];
    int key;
    printf("请选择您的操作:\n");
    scanf("%d", &key);

    switch(key)
    {
        case 1:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU");
            read_data(sql);
            getchar();
            getchar();
            show_stu(id, passwd);
            break;
        case 2:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by no");
            read_data(sql);
            getchar();
            getchar();
            show_stu(id, passwd);
            break;
        case 3:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by math desc");
            read_data(sql);
            sprintf(sql1,"select avg(math) from STU");
            read_data(sql1);
            getchar();
            getchar();
            show_stu(id, passwd);
            break;
        case 4:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by c desc");
            read_data(sql);
            sprintf(sql1,"select avg(c) from STU");
            read_data(sql1);
            getchar();
            getchar();
            show_stu(id, passwd);
            break;
        case 5:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by chinese desc");
            read_data(sql);
            sprintf(sql1,"select avg(chinese) from STU");
            read_data(sql1);
            getchar();
            getchar();
            show_stu(id, passwd);
            break;
        case 6:
            tea_menu(id, passwd);
            break;
        default :
            printf("您的输入有误，请重新输入！\n");
            getchar();
            getchar();
            show_stu(id, passwd);
    }
}/*}}}*/

//查找学生信息
void find_stu(int id, char* passwd)
{
    system("clear");

    printf("******************************\n");
    printf("*                            *\n");
    printf("*     1 按学号               *\n");
    printf("*                            *\n");
    printf("*     2 按姓名               *\n");
    printf("*                            *\n");
    printf("*     3 显示全班             *\n");
    printf("*                            *\n");
    printf("*     4 返回上级菜单         *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("*                            *\n");
    printf("******************************\n");

    int id1;
    char sql[100];
    int key;
    printf("请选择您的操作:\n");
    scanf("%d", &key);

    int _class;     
    char name[128]; 

    switch(key)
    {
        case 1:
            printf("请输入所要查找的学号:\n");
            scanf("%d", &id1);
            sprintf(sql,"select * from STU where id=%d",id1);
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            read_data(sql);
            getchar();
            getchar();
            find_stu(id, passwd);
            break;
        case 2:
            printf("请输入所要查找的姓名:\n");
            scanf("%s", name);
            sprintf(sql,"select * from STU where name='%s'",name);
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            read_data(sql);
            getchar();
            getchar();
            find_stu(id, passwd);
            break;     
        case 3:
            printf("请输入所要查看的班级:\n");
            scanf("%d", &_class);
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU where _class=%d",_class);
            read_data(sql);
            getchar();
            getchar();
            find_stu(id, passwd);
            break;
        case 4:
            tea_menu(id, passwd);
            break;
        default :
            printf("您的输入有误，请重新输入\n");
            getchar();
            getchar();
            find_stu(id, passwd);
    }
}

//修改学生信息
void set_stu(int id, char* passwd)
{ 
    int id1;
    char sql[100];
    int grade;
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    printf("请输入要修改学生的学号:\n");
    scanf("%d", &id1);
    sprintf(sql,"select * from STU where id=%d",id1);
    if(mysql_real_query(&mysql,sql,strlen(sql)) == 0)
    {
        system("clear");
        printf("******************************\n");
        printf("*                            *\n");
        printf("*     1 数学                 *\n");
        printf("*                            *\n");
        printf("*     2 C语言                *\n");
        printf("*                            *\n");
        printf("*     3 语文                 *\n");
        printf("*                            *\n");
        printf("*     4 返回上级菜单         *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("******************************\n");
        printf("\n请选择你的操作:\n");

        int key;
        scanf("%d", &key);

        switch(key)
        {
            case 1:
                printf("请输入修改后的成绩:\n");
                scanf("%d", &grade);
                sprintf(sql,"update STU set math=%d",grade);
                save_data(sql);
                no_sort();
                mysql_close(&mysql);
                getchar();
                getchar();
                set_stu(id, passwd);
                break;
            case 2:
                printf("请输入修改后的成绩:\n");
                scanf("%d", &grade);
                sprintf(sql,"update STU set c=%d",grade);
                save_data(sql);
                no_sort();
                mysql_close(&mysql);
                getchar();
                getchar();
                set_stu(id, passwd);
                break;
            case 3:
                printf("请输入修改后的成绩:\n");
                scanf("%d", &grade);
                sprintf(sql,"update STU set chinese=%d",grade);
                save_data(sql);
                no_sort();
                mysql_close(&mysql);
                getchar();
                getchar();
                set_stu(id, passwd);
                break;
            case 4 :
                mysql_close(&mysql);
                tea_menu(id,passwd);
                break;
            default :
                printf("您的输入有误，请重新输入\n");
                mysql_close(&mysql);
                getchar();
                getchar();
                set_stu(id, passwd);
        }
    }
    else
    {
        printf("该学号不存在!\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        set_stu(id,passwd);
    }
}

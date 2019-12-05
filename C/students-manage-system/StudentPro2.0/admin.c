#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "save.h"
#include "menu.h"
#include "admin.h"


//管理员菜单
void admin_menu()
{
    system("clear");

    printf("**********管理员登录**********\n");
    printf("*                            *\n");
    printf("*       1 查看学生信息       *\n");
    printf("*                            *\n");
    printf("*       2 查看老师信息       *\n");
    printf("*                            *\n");
    printf("*       3 查找学生信息       *\n");
    printf("*                            *\n");
    printf("*       4 修改学生信息       *\n");
    printf("*                            *\n");
    printf("*       5 修改老师信息       *\n");
    printf("*                            *\n");
    printf("*       6 删除学生信息       *\n");
    printf("*                            *\n");
    printf("*       7 删除老师信息       *\n");
    printf("*                            *\n");
    printf("*       8 返回上级菜单       *\n");
    printf("*                            *\n");
    printf("******************************\n");

    int key;
    printf("请选择你的操作:\n");
    scanf("%d", &key);

    int id;
    int i;


    switch(key)
    {
        case 1:
            show_student();
            break;
        case 2:
            show_teacher();   
            break;
        case 3:
            find_student();
            break;
        case 4:
            set_student();
            break;
        case 5:
            set_teacher();
            break;
        case 6:
            del_student();
            break;
        case 7:
            del_teacher();
            break;
        case 8:
            menu();
            break;
    }
}

void show_student()
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
    int key;
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    printf("请选择您的操作:\n");
    scanf("%d", &key);

    switch(key)
    {
        case 1:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            show_student();
            break;
        case 2:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by no");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            show_student();
            break;
        case 3:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by math desc");
            read_data(sql);
            sprintf(sql,"select avg(math) from STU");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            show_student();
            break;
        case 4:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by c desc");
            read_data(sql);
            sprintf(sql,"select avg(c) from STU");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            show_student();
            break;
        case 5:
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU order by chinese desc");
            read_data(sql);
            sprintf(sql,"select avg(chinese) from STU");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            show_student();
            break;
        case 6:
            mysql_close(&mysql);
            admin_menu();
            break;
        default :
            printf("您的输入有误，请重新输入！\n");
            mysql_close(&mysql);
            getchar();
            getchar();
            show_student();
    }
}

//查找学生信息
void find_student()
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
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
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
            mysql_close(&mysql);
            getchar();
            getchar();
            find_student();
            break;
        case 2:
            printf("请输入所要查找的姓名:\n");
            scanf("%s", name);
            sprintf(sql,"select * from STU where name='%s'",name);
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            find_student();
            break;     
        case 3:
            printf("请输入所要查看的班级:\n");
            scanf("%d", &_class);
            printf("学号\t姓名\t密码\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            sprintf(sql,"select * from STU where _class=%d",_class);
            read_data(sql);
            mysql_close(&mysql);
            getchar();
            getchar();
            find_student();
            break;
        case 4:
            mysql_close(&mysql);
            admin_menu();
            break;
        default :
            printf("您的输入有误，请重新输入\n");
            mysql_close(&mysql);
            getchar();
            getchar();
            find_student();
    }
}


void set_student()
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
        printf("*     4 修改姓名             *\n");
        printf("*                            *\n");
        printf("*     5 修改班级             *\n");
        printf("*                            *\n");
        printf("*     6 返回上级菜单         *\n");
        printf("*                            *\n");
        printf("******************************\n");
        printf("\n请选择需要修改的内容:\n");

        int key;
        scanf("%d", &key);
        char name[128];
        int _class;
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
                set_student();
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
                set_student();
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
                set_student();
                break;
            case 4 :
                printf("请输入修改后的姓名:\n");
                scanf("%s", name);
                sprintf(sql,"update STU set name='%s' where id=%d",name,id1);
                save_data(sql);
                mysql_close(&mysql);
                getchar();
                getchar();
                set_student();
                break;
            case 5:
                printf("请输入修改后的班级:\n");
                scanf("%d", &_class);
                sprintf(sql,"update STU set _class=%d where id=%d",_class,id1);
                mysql_close(&mysql);
                getchar();
                getchar();
                set_student();
                break;
            case 6:
                mysql_close(&mysql);
                admin_menu();
                break;
            default :
                printf("您的输入有误，请重新输入\n");
                mysql_close(&mysql);
                getchar();
                getchar();
                set_student();
        }
    }
    else
    {
        printf("该学号不存在!\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        set_student();
    }
}



//显示老师信息
void show_teacher()
{
    char sql[100];
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    printf("ID\t姓名\t密码\t班级\n");
    sprintf(sql,"select * from TEA");
    read_data(sql);
    mysql_close(&mysql);
    getchar();
    getchar();
    admin_menu();
}

//修改老师信息
void set_teacher()
{
    int id;
    char sql[100];
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    printf("请输入老师ID:\n");
    scanf("%d",&id);
    sprintf(sql,"select * from TEA where id=%d",id);
    if(mysql_real_query(&mysql,sql,strlen(sql)) == 0)
    {
        system("clear");
        printf("******************************\n");
        printf("*                            *\n");
        printf("*     1 修改姓名             *\n");
        printf("*                            *\n");
        printf("*     2 修改密码             *\n");
        printf("*                            *\n");
        printf("*     3 修改班级             *\n");
        printf("*                            *\n");
        printf("*     4 返回上级菜单         *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("*                            *\n");
        printf("******************************\n");
        int key;
        scanf("%d", &key);

        char passwd1[32];
        char name[128];
        int _class;

        switch(key)
        {
            case 1:
                printf("请输入新的名字:\n");
                scanf("%s", name);
                sprintf(sql,"update TEA set name='%s' where id=%d",name,id);
                save_data(sql);
                mysql_close(&mysql);
                getchar();
                getchar();
                admin_menu();
                break;
            case 2:
                printf("请输入新的密码:\n");
                scanf("%s", passwd1);
                sprintf(sql,"update TEA set passwd='%s' where id=%d",passwd1,id);
                save_data(sql);
                mysql_close(&mysql);
                getchar();
                getchar();
                admin_menu();
                break;
            case 3:
                printf("请输入新的班级:\n");
                scanf("%d", &_class);
                sprintf(sql,"update TEA set _class=%d where id=%d",_class,id);
                save_data(sql);
                mysql_close(&mysql);
                getchar();
                getchar();
                admin_menu();
                break;
            case 4:
                mysql_close(&mysql);
                admin_menu();
                break;
        }
    }
    else
    {
        printf("该老师不存在!\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        admin_menu();
    }
}

//删除学生信息   no sort
void del_student()
{
    printf("请输入要删除学生的学号:\n");
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    int id;
    scanf("%d", &id);

    char sql[100];
    sprintf(sql,"select * from STU where id=%d",id);
    if(mysql_real_query(&mysql,sql,strlen(sql)) == 0)
    {
        sprintf(sql,"delete from STU where id=%d",id);
        save_data(sql);
        no_sort();
        mysql_close(&mysql);
        admin_menu();
    }
    else
    {
        printf("该学号不存在!\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        admin_menu();
    }
}

//删除老师信息
void del_teacher()
{
    printf("请输入要删除老师的ID:\n");
    MYSQL mysql;
    mysql_init(&mysql);                            
    if(!mysql_real_connect(&mysql,"localhost","root","123456","student",0,NULL,0))                    
        printf("database connect fail!\n");        
    int id;
    scanf("%d", &id); 

    char sql[100];
    sprintf(sql,"select * from TEA where id=%d",id);
    if(mysql_real_query(&mysql,sql,strlen(sql)) == 0)
    {
        sprintf(sql,"delete from TEA where id=%d",id);
        save_data(sql);
        mysql_close(&mysql);
        admin_menu();
    }
    else
    {
        printf("该老师不存在!\n");
        mysql_close(&mysql);
        getchar();
        getchar();
        admin_menu();
    }
}


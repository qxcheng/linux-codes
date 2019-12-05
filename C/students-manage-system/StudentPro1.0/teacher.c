#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "student.h"
#include "teacher.h"
#include "save.h"
#include "common.h"
#include "admin.h"

//老师界面菜单
void tea_menu(int i)
{
    system("clear");

    printf("********欢迎 %3s 老师登录*****\n", TEA[i].name);
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

    char pass4[32];
    int index;
    int j;
    int key;
    scanf("%d", &key);
    switch(key)
    {
        case 1:
            printf("姓名:%s\n", TEA[i].name);
            printf("编号:%d\n", TEA[i].id);
            printf("班级:%d\n\n", TEA[i]._class);
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            tea_menu(i);
            break;
        case 2:
            printf("请输入原密码:\n");
            scanf("%s", pass4);
            if(strcmp(pass4, TEA[i].passwd)==0)
            {
                printf("请输入新密码:\n");
                scanf("%s", TEA[i].passwd);
                write_tea(len_tea);
                printf("修改密码成功!\n");
                printf("请按任意键继续!\n");
                getchar();
                getchar();
                tea_menu(i);
            }
            else
            {
                printf("原密码错误,请重新输入!\n");
                printf("请按任意键继续!\n");
                getchar();
                getchar();
                tea_menu(i);
            }
            break;
        case 3:
            show_stu(i, len_stu);
            break;
        case 4:
            find_stu(i, len_stu);
            break;
        case 5:
            printf("请输入学生的学号:\n");
            scanf("%d", &index);
            for(j=0; j<len_stu; j++)
            {
                if(index == STU[j].id)
                {
                    set_stu(i,j);
                }
            }
            printf("该学号不存在!\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            tea_menu(i);
            break;
        case 6:
            menu();
            break;
        default :
            printf("您的输入有误，请重新输入！\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            tea_menu(i);
    }

}

//显示学生信息
void show_stu(int i, int len)
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

    int key;
    printf("请选择您的操作:\n");
    scanf("%d", &key);

    int i1, j;
    float ave;
    struct student temp;
    struct student TEMP[100];
    for(i1=0; i1<len_stu; i1++)
    {
        TEMP[i1] = STU[i1];     
    }

    switch(key)
    {
        case 1:
            printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            for(i1=0; i1<len; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);
            }
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_stu(i, len_stu);
            break;
        case 2:
            for(i1=0; i1<len-1; i1++)
            {
                for(j=0; j<len-1-i1; j++)
                {
                    if(TEMP[j].no > TEMP[j+1].no)     
                    {
                        temp = TEMP[j];
                        TEMP[j] = TEMP[j+1];
                        TEMP[j+1] = temp;
                    }
                }
            }
            printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            for(i1=0; i1<len; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_stu(i, len_stu);
            break;
        case 3:
            for(i1=0; i1<len-1; i1++)
            {
                for(j=0; j<len-1-i1; j++)
                {
                    if(TEMP[j].math < TEMP[j+1].math)     
                    {
                        temp = TEMP[j];
                        TEMP[j] = TEMP[j+1];
                        TEMP[j+1] = temp;
                    }
                }
            }
            printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            for(i1=0; i1<len; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            ave = 0;
            for(i1=0; i1<len; i1++)
            {
                ave += STU[i1].math;
            }
            ave /= len;
            printf("数学成绩平均分:%.2f\n\n", ave);
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_stu(i, len_stu);
            break;
        case 4:
            for(i1=0; i1<len-1; i1++)
            {
                for(j=0; j<len-1-i1; j++)
                {
                    if(TEMP[j].c < TEMP[j+1].c)     
                    {
                        temp = TEMP[j];
                        TEMP[j] = TEMP[j+1];
                        TEMP[j+1] = temp;
                    }
                }
            }
            printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            for(i1=0; i1<len; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            ave = 0;
            for(i1=0; i1<len; i1++)
            {
                ave += STU[i1].c;
            }
            ave /= len;
            printf("C语言成绩平均分:%.2f\n\n", ave);
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_stu(i, len_stu);
            break;
        case 5:
            for(i1=0; i1<len-1; i1++)
            {
                for(j=0; j<len-1-i1; j++)
                {
                    if(TEMP[j].chinese < TEMP[j+1].chinese)     
                    {
                        temp = TEMP[j];
                        TEMP[j] = TEMP[j+1];
                        TEMP[j+1] = temp;
                    }
                }
            }
            printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            for(i1=0; i1<len; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            ave = 0;
            for(i1=0; i1<len; i1++)
            {
                ave += STU[i1].chinese;
            }
            ave /= len;
            printf("语文成绩平均分:%.2f\n\n", ave);
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_stu(i, len_stu);
            break;
        case 6:
            tea_menu(i);
            break;
        default :
            printf("您的输入有误，请重新输入！\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            show_stu(i, len_stu);
    }
}

//查找学生信息
void find_stu(int i, int len)
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

    int key;
    printf("请选择您的操作:\n");
    scanf("%d", &key);

    int i1;
    int id;
    int _class;
    char name[128];
    int num=0;
    switch(key)
    {
        case 1:
            printf("请输入所要查找的学号:\n");
            scanf("%d", &id);
            for(i1=0; i1<len; i1++)
            {
                if(STU[i1].id == id)
                {
                    printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
                    printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);
                    printf("请按任意键继续\n");
                    getchar();
                    getchar();
                    find_stu(i, len_stu);
                    break;     
                }
            }
            printf("未找到该学生信息！\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            find_stu(i, len_stu);
            break;
        case 2:
            printf("请输入所要查找的姓名:\n");
            scanf("%s", name);
            for(i1=0; i1<len; i1++)
            {
                if(strcmp(STU[i1].name, name) == 0)
                {
                    num++;
                    if(1 == num)
                    {
                        printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
                        printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);
                    }
                    else
                    {
                        printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);
                    }
                }
            }
            if(0 == num)
            {    
                printf("未找到该学生信息！\n");
                printf("请按任意键继续!\n");
                getchar();
                getchar();
                find_stu(i, len_stu);
            }
            else
            {
                printf("请按任意键继续\n");
                getchar();
                getchar();
                find_stu(i, len_stu);
                break;     
            }
            break;
        case 3:
            printf("请输入所要查看的班级:\n");
            scanf("%d", &_class);
            printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            for(i1=0; i1<len; i1++)
            {
                if(STU[i1]._class == _class)
                {
                    printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);  
                }
            }
            printf("请按任意键继续\n");
            getchar();
            getchar();
            find_stu(i, len_stu);
            break;
        case 4:
            tea_menu(i);
            break;
        default :
            printf("您的输入有误，请重新输入\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            find_stu(i, len_stu);
    }
}

//修改学生信息
void set_stu(int i, int i1)
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
    printf("\n请选择需要修改的成绩:\n");

    int key;
    scanf("%d", &key);

    switch(key)
    {
        case 1:
            printf("原成绩为: %d ,请输入修改后的成绩:\n", STU[i1].math);
            scanf("%d", &STU[i1].math);
            grade_sort(len_stu);
            write_stu(len_stu);
            printf("修改成功!\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            set_stu(i, i1);
            break;
        case 2:
            printf("原成绩为: %d ,请输入修改后的成绩:\n", STU[i1].c);
            scanf("%d", &STU[i1].c);
            grade_sort(len_stu);
            write_stu(len_stu);
            printf("修改成功!\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            set_stu(i, i1);
            break;
        case 3:
            printf("原成绩为: %d ,请输入修改后的成绩:\n", STU[i1].chinese);
            scanf("%d", &STU[i1].chinese);
            grade_sort(len_stu);
            write_stu(len_stu);
            printf("修改成功!\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            set_stu(i, i1);
            break;
        case 4 :
            tea_menu(i);
            break;
        default :
            printf("您的输入有误，请重新输入\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            set_stu(i, i1);
    }
}

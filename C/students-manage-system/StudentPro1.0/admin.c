#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "menu.h"
#include "common.h"
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
    
    void (*p)(void);
    p = show_teacher;

    switch(key)
    {
        case 1:
            show_student();
            break;
        case 2:
            p();   //show_teacher();
            break;
        case 3:
            find_student();
            break;
        case 4:
            printf("请输入学生的学号:\n");
            scanf("%d", &id);
            for(i=0; i<len_stu; i++)
            {
                if(id == STU[i].id)
                {
                    set_student(i);
                }
            }
            printf("学生ID不存在!\n");
            printf("按任意键继续!\n");
            getchar();
            getchar();
            admin_menu();
            break;
        case 5:
            printf("请输入老师的ID\n");
            scanf("%d", &id);
            for(i=0; i<len_tea; i++)
            {
                if(id == TEA[i].id)
                {
                    set_teacher(i);
                }
            }
            printf("老师ID不存在!\n");
            printf("按任意键继续!\n");
            getchar();
            getchar();
            admin_menu();
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

//显示学生信息
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
            for(i1=0; i1<len_stu; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);
            }
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_student();
            break;
        case 2:
            for(i1=0; i1<len_stu-1; i1++)
            {
                for(j=0; j<len_stu-1-i1; j++)
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
            for(i1=0; i1<len_stu; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_student();
            break;
        case 3:
            for(i1=0; i1<len_stu-1; i1++)
            {
                for(j=0; j<len_stu-1-i1; j++)
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
            for(i1=0; i1<len_stu; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            ave = 0;
            for(i1=0; i1<len_stu; i1++)
            {
                ave += STU[i1].math;
            }
            ave /= len_stu;
            printf("数学成绩平均分:%.2f\n\n", ave);
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_student();
            break;
        case 4:
            for(i1=0; i1<len_stu-1; i1++)
            {
                for(j=0; j<len_stu-1-i1; j++)
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
            for(i1=0; i1<len_stu; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            ave = 0;
            for(i1=0; i1<len_stu; i1++)
            {
                ave += STU[i1].c;
            }
            ave /= len_stu;
            printf("C语言成绩平均分:%.2f\n\n", ave);
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_student();
            break;
        case 5:
            for(i1=0; i1<len_stu-1; i1++)
            {
                for(j=0; j<len_stu-1-i1; j++)
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
            for(i1=0; i1<len_stu; i1++)
            {
                printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", TEMP[i1].id, TEMP[i1].name, TEMP[i1].age, TEMP[i1]._class, TEMP[i1].math, TEMP[i1].chinese, TEMP[i1].c, TEMP[i1].no);
            }
            ave = 0;
            for(i1=0; i1<len_stu; i1++)
            {
                ave += STU[i1].chinese;
            }
            ave /= len_stu;
            printf("语文成绩平均分:%.2f\n\n", ave);
            printf("按任意键继续\n");
            getchar();
            getchar();
            show_student();
            break;
        case 6:
            admin_menu();
            break;
        default :
            printf("您的输入有误，请重新输入！\n");
            printf("请按任意键继续!\n");
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

    int key;
    printf("请选择您的操作:\n");
    scanf("%d", &key);

    int i1;
    int id;
    int _class;
    char name[128];

    int num=0; //number of students finding by name

    switch(key)
    {
        case 1:
            printf("请输入所要查找的学号:\n");
            scanf("%d", &id);
            for(i1=0; i1<len_stu; i1++)
            {
                if(STU[i1].id == id)
                {
                    printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
                    printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);
                    printf("请按任意键继续\n");
                    getchar();
                    getchar();
                    find_student();
                    break;     
                }
            }
            printf("未找到该学生信息！\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            find_student();
            break;
        case 2:
            printf("请输入所要查找的姓名:\n");
            scanf("%s", name);
            for(i1=0; i1<len_stu; i1++)
            {
                if(strcmp(STU[i1].name, name) == 0)
                {
                    num++;
                    if(1 == num)
                    {printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
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
                find_student();
                break;
            }
            else
            {
                printf("请按任意键继续\n");
                getchar();
                getchar();
                find_student();
                break;     
            }
        case 3:
            printf("请输入所要查看的班级:\n");
            scanf("%d", &_class);
            printf("学号\t姓名\t年龄\t班级\t数学\t语文\tC语言\t名次\n");
            for(i1=0; i1<len_stu; i1++)
            {
                if(STU[i1]._class == _class)
                {
                    printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", STU[i1].id, STU[i1].name, STU[i1].age, STU[i1]._class, STU[i1].math, STU[i1].chinese, STU[i1].c, STU[i1].no);  
                }
            }
            printf("请按任意键继续\n");
            getchar();
            getchar();
            find_student();
            break;
        case 4:
            admin_menu();
            break;
        default :
            printf("您的输入有误，请重新输入\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            find_student();
    }
}

//修改学生信息
void set_student(int i1)
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
            printf("修改成功,按任意键继续!\n");
            getchar();
            getchar();
            set_student(i1);
            break;
        case 2:
            printf("原成绩为: %d ,请输入修改后的成绩:\n", STU[i1].c);
            scanf("%d", &STU[i1].c);
            grade_sort(len_stu);
            write_stu(len_stu);
            printf("修改成功,按任意键继续!\n");
            getchar();
            getchar();
            set_student(i1);
            break;
        case 3:
            printf("原成绩为: %d ,请输入修改后的成绩:\n", STU[i1].chinese);
            scanf("%d", &STU[i1].chinese);
            grade_sort(len_stu);
            write_stu(len_stu);
            printf("修改成功,按任意键继续!\n");
            getchar();
            getchar();
            set_student(i1);
            break;
        case 4:
            printf("原姓名为: %s ,请输入修改后的姓名:\n", STU[i1].name);
            scanf("%s", STU[i1].name);
            write_stu(len_stu);
            printf("修改成功,按任意键继续!\n");
            getchar();
            getchar();
            set_student(i1);
            break;
        case 5:
            printf("原班级为: %d ,请输入修改后的班级:\n", STU[i1]._class);
            scanf("%d", &STU[i1]._class);
            write_stu(len_stu);
            printf("修改成功,按任意键继续!\n");
            getchar();
            getchar();
            set_student(i1);
            break;
        case 6:
            admin_menu();
            break;
        default :
            printf("您的输入有误，请重新输入\n");
            printf("请按任意键继续!\n");
            getchar();
            getchar();
            set_student(i1);
    }
}

//显示老师信息
void show_teacher()
{
    printf("姓名\tID\t班级\n");
    int i;
    for(i=0; i<len_tea; i++)
    {
        printf("%s\t%d\t%d\n", TEA[i].name,TEA[i].id,TEA[i]._class);
    }
    printf("请按任意键继续!\n");
    getchar();
    getchar();
    admin_menu();
}

//修改老师信息
void set_teacher(int i)
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

    char pass2[32];

    switch(key)
    {
        case 1:
            printf("请输入新的名字:\n");
            scanf("%s", TEA[i].name);
            write_tea(len_tea);
            printf("修改成功,按任意键继续!\n");
            getchar();
            getchar();
            admin_menu();
            break;
        case 2:
            printf("请输入原密码:\n");
            scanf("%s", pass2);
            if(strcmp(pass2, TEA[i].passwd)==0)
            {
                printf("请输入新的密码:\n");
                scanf("%s", TEA[i].passwd);
                write_tea(len_tea);
                printf("修改成功,按任意键继续!\n");
                getchar();
                getchar();
                admin_menu();
                break;
            }
            else
            {
                printf("原密码错误!\n");
                printf("按任意键继续!\n");
                getchar();
                getchar();
                admin_menu();
            }
        case 3:
            printf("请输入新的班级:\n");
            scanf("%d", &TEA[i]._class);
            write_tea(len_tea);
            printf("修改成功,按任意键继续!\n");
            getchar();
            getchar();
            admin_menu();
            break;
        case 4:
            admin_menu();
            break;
    }
}

//删除学生信息
void del_student()
{
    printf("请输入要删除学生的学号:\n");
    int id;
    scanf("%d", &id);

    int i;
    for(i=0; i<len_stu; i++)
    {
        if(STU[i].id == id)
        {
            for(int j=i; j<len_stu-1; j++)
            {
                STU[j] = STU[j+1];
            }
            len_stu--;
            grade_sort(len_stu);
            write_stu(len_stu);
            printf("删除成功，按任意键继续!\n");
            getchar();
            getchar();
            admin_menu();
        }
    }
    printf("该学号不存在，请重新输入!");
    printf("按任意键继续!\n");
    getchar();
    getchar();
    del_student();
}

//删除老师信息
void del_teacher()
{
    printf("请输入要删除老师的ID:\n");
    int id;
    scanf("%d", &id); 

    int i;
    for(i=0; i<len_tea; i++)
    {
        if(TEA[i].id == id)
        {
            for(int j=i; j<len_tea-1; j++)
            {
                TEA[j] = TEA[j+1];
            }
            len_tea--;
            write_tea(len_tea);
            printf("删除成功，按任意键继续!\n");
            getchar();
            getchar();
            admin_menu();
        }
    }
    printf("该ID不存在，请重新输入!");
    printf("按任意键继续!\n");
    getchar();
    getchar();
    del_teacher();  
}

//修改学生的名次
void grade_sort(int len)
{
    int i, j;
    struct student temp[100];
    struct student TEMP;
    for(i=0; i<len; i++)
    {
        temp[i] = STU[i];
    }
    for(i=0; i<len-1; i++)
    {
        for(j=0; j<len-i-1; j++)
        {
            if(temp[j].math+temp[j].c+temp[j].chinese < temp[j+1].math+temp[j+1].c+temp[j+1].chinese)
            {
                TEMP = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = TEMP;
            }
        }
    }
    for(i=0; i<len; i++)
    {
        temp[i].no = i+1;
    }
    for(i=0; i<len; i++)
    {
        for(j=0; j<len; j++)
        {
            if(temp[i].id == STU[j].id)
            {
                STU[j].no = temp[i].no;
            }
        }
    }
}

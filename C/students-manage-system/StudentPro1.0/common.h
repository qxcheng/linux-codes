#ifndef __COMMON_H__
#define __COMMON_H__

typedef struct student
{
    int id;              
    char name[128];      
    char passwd[32];
    int age;
    int _class;
    int math;
    int c;
    int chinese;
    int no;
}student;

typedef struct teacher
{
    int id;
    char name[128];
    char passwd[32];
    int _class;
}teacher;

extern int len_stu;
extern int len_tea;
extern struct teacher TEA[];
extern struct student STU[];


#endif

#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int Status;
typedef int Elemtype;

//存储结构
typedef struct DualNode
{
    Elemtype data;
    struct DualNode *prior; //前驱结点
    struct DualNode *next;  //后继结点
}DualNode, *DuLinkList;

//初始化
Status InitList(DuLinkList *L)
{
    *L = (DuLinkList)malloc(sizeof(DualNode));
    if(!(*L))
        return ERROR;
    (*L)->next = (*L)->prior = NULL;
    return OK;
}

//在第i个位置插入元素
Status InsertElem(DuLinkList L, int i, Elemtype e)
{
    DuLinkList temp, p=L;
    int j=0;
    while(p && j<(i-1))
    {
        p = p->next;
        j++;
    }
    if(!p || j>i-1)
        return ERROR;
    temp = (DuLinkList)malloc(sizeof(DualNode));
    if(!temp)
        return ERROR;
    temp->data = e;
    temp->next = p->next;
    temp->prior = p;
    if(p->next)
        p->next->prior = temp;
    p->next = temp;
    return OK;
}

//删除第i个结点并返回其值
Status DeleteElem(DuLinkList L, int i, Elemtype *e)
{
    DuLinkList p=L;
    int j=0;
    while(p && j<i)
    {
        p = p->next;
        j++;
    }
    if(!p || j>i)
        return ERROR;
    *e = p->data;
    p->prior->next = p->next;
    p->next->prior = p->prior;
    free(p);
    return OK;
}

//遍历
void traverse(DuLinkList L)
{
    DuLinkList p=L->next;
    while(p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

int main()
{
    DuLinkList L;
    Elemtype *e;
    if(InitList(&L))
    {
        for(int i=1; i<10; i++)
            InsertElem(L, i, i);
        traverse(L);
        DeleteElem(L, 5, e);
        printf("%d\n", *e);
        traverse(L);
    }
    return 0;
}

#if 0  // by me

#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
    int data;
    struct node *prior;
    struct node *next;
}LNode, *LinkList;

void create(LinkList *L);
void insert(LinkList L, int i, int data);
int del(LinkList L, int i);
LinkList find_val(LinkList L, int data);
LinkList find_ind(LinkList L, int i);
void traverse(LinkList L);
int len(LinkList L);


void create(LinkList *L)
{
    *L = (LinkList) malloc(sizeof(LNode));
    (*L)->prior = NULL;
    (*L)->next  = NULL;
    
    int data;
    scanf("%d", &data);
    while(data>0)
    {
        LinkList temp = (LinkList) malloc(sizeof(LNode));
        temp->data = data;
        if((*L)->next)
        {
            temp->next = (*L)->next;
            temp->prior = *L;
            (*L)->next->prior = temp;
            (*L)->next = temp;
        }
        else
        {
            temp->next = (*L)->next;
            temp->prior = *L;
            (*L)->next = temp;
        }
        scanf("%d", &data);
    }
    return ;
}

void traverse(LinkList L)
{
    LinkList temp = L;
    printf("-->");
    while(temp->next)
    {
        temp = temp->next;
        printf("%3d", temp->data);
    }
    printf("\n<--");
    while(temp!=L)
    {
        printf("%3d", temp->data);
        temp = temp->prior;
    }
    printf("\n");
}

int len(LinkList L)
{
    int n=0;
    LinkList temp = L->next;
    while(temp)
    {
        n++;
        temp = temp->next;
    }
    return n;
}

void insert(LinkList L, int i, int data)
{
    LinkList temp = L;
    if(i<1 || i>len(L)+1)
    {
        return ;
    }
    LinkList s = (LinkList) malloc(sizeof(LNode));
    s->data = data;
    if(i == len(L)+1)
    {
        for(int j=1; j<i; j++)
        {
            temp = temp->next;
        }
        s->next = temp->next;
        s->prior = temp;
        temp->next = s;
    }
    else
    {
        for(int j=0; j<i; j++)
        {
            temp = temp->next;
        }
        s->next = temp;
        s->prior = temp->prior;
        temp->prior->next = s;
        temp->prior = s;
    }
}

int del(LinkList L, int i)
{   
    if(i<1 || i>len(L))
    {
        return -1;
    }
    LinkList temp = L;
    for(int j=0; j<i; j++)
    {
        temp = temp->next;
    }
    int result = temp->data;
    if(i == len(L))
    {
        temp->prior->next = temp->next;
        free(temp);
    }
    else
    {
        temp->prior->next = temp->next;
        temp->next->prior = temp->prior;
        free(temp);
    }
    return result;
}

LinkList find_val(LinkList L, int data)
{
    LinkList temp = L->next;
    while(temp)
    {
        if(temp->data == data)
        {
            return temp;
        }
        temp = temp->next;
    }
    return L;
}

LinkList find_ind(LinkList L, int i)
{
    LinkList temp = L;
    if(i<1 || i>len(L))
    {
        return L;
    }
    for(int j=0; j<i; j++)
    {
        temp = temp->next;
    }
    return temp;
}

int main()
{   
    LinkList L;
    create(&L);
    traverse(L);
    printf("\n");

    insert(L,1,10);
    traverse(L);
    printf("\n");
    insert(L,7,20);
    traverse(L);
    printf("\n");
    
    int val1 = del(L, 1);
    printf("%d\n", val1);
    traverse(L);
    printf("\n");
    int val2 = del(L, 6);
    printf("%d\n", val2);
    traverse(L);
    printf("\n");
    
    LinkList p1 = find_val(L, 5);
    printf("%d\n", p1->data);

    LinkList p2 = find_ind(L, 3);
    printf("%d\n", p2->data);

    return 0;
}
#endif

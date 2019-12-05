#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int Status;
typedef int Elemtype;

typedef struct LNode
{
    Elemtype data;
    struct LNode *next;
}LNode, *LinkList;

/*init*/
Status InitList(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (!(*L))
        return ERROR;
    (*L)->next = *L;
    return OK;
}

//get length
int ListLength(LinkList L)
{
    int i=0;
    LinkList p=L->next;
    while(p!=L)
    {
        i++;
        p = p->next;
    }
    return i;
}

//insert an elem at ith
Status InsertElem(LinkList L, int i, Elemtype e)
{
    LinkList temp, p;
    p = L;
    int j=0;
    if(i<1 || i>(ListLength(L)+1))
        return ERROR;

    temp = (LinkList)malloc(sizeof(LNode));
    if(!temp)
        return ERROR;
    temp->data = e;
    for(; j<(i-1); j++)
        p = p->next;
    temp->next = p->next;
    p->next = temp;
}

//delete ith elem
Status DeleteElem(LinkList L, int i, Elemtype *e)
{
    LinkList p=L;
    int j=0;
    if(i<1 || i>ListLength(L))
        return ERROR;
    for(; j<(i-1); j++)
        p = p->next;
    *e = p->next->data;
    LinkList temp = p->next;
    p->next = p->next->next;
    free(temp);
    return OK;
}

//traverse list
void traverse(LinkList L)
{
    LinkList temp=L->next;
    while(temp != L)
    {
        printf("%d  ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}


int main()
{
    LinkList L;
    int i;
    Elemtype e;
    if(InitList(&L))
    {
        for(int j=1; j<10; j++)
            InsertElem(L, j, j);
        i = ListLength(L);
        printf("%d\n", i);
        traverse(L);
        DeleteElem(L, 10, &e);
        printf("%d\n", e);
        i = ListLength(L);
        printf("%d\n", i);
        traverse(L);
    }
    return 0;
}

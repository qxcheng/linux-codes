#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0
#define MAXSIZE 1000

typedef int Status;
typedef int Elemtype;

//save struct
typedef struct
{
    Elemtype data;
    int cur;
}Component, StaticLinkList[MAXSIZE];

//init
Status InitList(StaticLinkList L)
{
    int i;
    for(i=0; i<MAXSIZE-1; i++)
        L[i].cur = i + 1;
    L[MAXSIZE-1].cur = 0;
    return OK;
}

/* get length */
int ListLength(StaticLinkList L)
{
    int j = 0;
    int i = L[MAXSIZE-1].cur;
    while(i)
    {
        i = L[i].cur;
        j++;
    }
    return j;
}

//
int Malloc_SLL(StaticLinkList L)
{
    int i = L[0].cur;
    if(L[0].cur)
        L[0].cur = L[i].cur;
    return i;
}

/* insert */
Status InsertElem( StaticLinkList L, int i, Elemtype e )
{
    int j, k, l;
    k = MAXSIZE - 1;    
    if( i<1 || i>ListLength(L)+1 )
    {
        return ERROR;
    }
    j = Malloc_SLL(L);
    if( j )
    {
        L[j].data = e;
        for( l=1; l <= i-1; l++ )
        {
            k = L[k].cur;
        }
        L[j].cur = L[k].cur;
        L[k].cur = j;
        return OK;
    }
    return ERROR;
}

/* delete */
Status DeleteElem(StaticLinkList L, int i)
{
    int j, k;
    if( i<1 || i>ListLength(L) )
    {
        return ERROR;
    }
    k = MAXSIZE - 1;
    for( j=1; j <= i-1; j++ )
    {
        k = L[k].cur;    // k1 = 1, k2 = 5
    }
    j = L[k].cur;        // j = 2
    L[k].cur = L[j].cur;
    Free_SLL(L, j);
    return OK;
}

/*  */
void Free_SLL(StaticLinkList L, int k)
{
    L[k].cur = L[0].cur;
    L[0].cur = k;
}


int main()
{
    StaticLinkList L;
    InitList(L);
    for(int i=1; i<10; i++)
        InsertElem(L, i, i);
    printf("%d\n", L[6].data);
    DeleteElem(L, 4);
    printf("%d", ListLength(L));
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#define OK    1
#define ERROR 0

typedef int Status;
typedef int Elemtype;

typedef struct SqList
{
    Elemtype *elem;
    int length;
    int size;
}SqList;

//init a SqList
Status init(SqList *L)
{
    L->elem = (Elemtype *) malloc(sizeof(Elemtype)*30);
    if (!L->elem)
        return ERROR;
    L->length = 0;
    L->size = 30;
    return OK;
}

//insert an elem to a SqList
Status insert(SqList *L, int i, Elemtype e)
{
    if (i<1 || i>L->length+1)
        return ERROR;
    Elemtype *p = &L->elem[i-1];
    Elemtype *q = &L->elem[L->length-1];
    for (; q>=p; q--)
        *(q+1) = *q;
    L->elem[i-1] = e;
    ++L->length;
    return OK;
}

//delete an elem from a SqList
Status del(SqList *L, int i, Elemtype *e)
{
    if (L->length == 0)
        return ERROR;
    if (i<1 || i>L->length)
        return ERROR;
    *e = L->elem[i-1];
    Elemtype *p = &L->elem[i-1];
    Elemtype *q = &L->elem[L->length-1];
    for(; p<q; p++)
        *p = *(p+1);
    --L->length;
    return OK;
}

//get an elem from a SqList by its position
Status get(SqList L, int i, Elemtype *e)
{
    if (i<1 || i>L.length || L.length==0)
        return ERROR;
    *e = L.elem[i-1];
    return OK;
}

int main()
{
    SqList L;
    init(&L);
    insert(&L,1,5);
    insert(&L,2,10);
    insert(&L,3,15);
    
    int e;
    del(&L,3,&e);
    printf("%d\n", e);
    get(L,2,&e);
    printf("%d\n", e);
}

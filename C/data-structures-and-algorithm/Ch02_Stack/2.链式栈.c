#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int ElemType;
typedef int Status;

/* 存储结构 */
typedef struct LNode
{
    ElemType data;
    struct LNode *next;
}LNode, *LinkList;

typedef struct
{
    LNode *base;    //栈尾指针
    LNode *top;     //栈顶指针
    int size;       //栈的大小
}LStack;

/* 初始化栈 */
Status InitStack(LStack *S)
{
    S->base = (LinkList) malloc(sizeof(LNode));
    if(!S->base)
        exit(OVERFLOW);
    S->base->data = 0;
    S->base->next = NULL;
    S->top = S->base;
    S->size = 0;
    return OK;
}

/* 压栈 */
Status Push(LStack *S, ElemType e)
{
    LNode *temp = (LinkList) malloc(sizeof(LNode));
    if(!temp)
        exit(OVERFLOW);
    temp->data = e;
    temp->next = NULL;
    S->top->next = temp;
    S->top = temp;
    S->size++;
    return OK;
}

/* 弹栈 */
Status Pop(LStack *S, ElemType *e)
{
    if(S->size != 0)
    {
        *e = S->top->data;
        LNode *temp = S->base;
        while(temp->next != S->top)  //temp指向S->top前一个结点
        {
            temp = temp->next;
        }
        LNode *p = temp->next;
        temp->next = NULL;
        free(p);
        S->top = temp;
    }
    else
    {
        printf("Warning: Empty Stack!");
        exit(OVERFLOW);
    }
    return OK;
}

/* 判断栈是否为空 */
Status IsEmpty(LStack S)
{
    return (S.size == 0);
}

/* 获取栈的长度 */
int GetLength(LStack S)
{
    return S.size;
}

/* 获取栈顶元素 */
Status GetTop(LStack *S, ElemType *e)
{
    if(S->size != 0)
        return S->top->data;
    else
    {
        printf("Warning: Empty Stack!");
        exit(OVERFLOW);
    }
    return OK;
}

/* 遍历栈 */
Status TraverseStack(LStack *S)
{
    if(S->size != 0)
    {
        LNode *temp = S->base->next;
        while(temp)
        {
            printf("%d ", temp->data);    //此处如何写成通用？？？
            temp = temp->next;
        }
    }
    else
    {
        printf("Warning: Empty Stack!");
        exit(OVERFLOW);
    }
    return OK;
}

/* 销毁栈 */
Status DestroyStack(LStack *S)
{   
    ; 
}

/* 清空栈 */
Status ClearStack(LStack *S)
{
    ; 
}

int main()
{
    LStack s;
    InitStack(&s);
    for(int i=0; i<10; i++)
    {
        Push(&s,i);
    }
    TraverseStack(&s);

    int e;
    Pop(&s,&e);
    printf("%d\n", e);
    TraverseStack(&s);

    return 0;
}
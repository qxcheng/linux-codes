#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2

#define MAXSIZE 100

typedef int ElemType;
typedef int Status;

/* 存储结构 */
typedef struct
{
    ElemType *base;
    int front;       //队头
    int rear;        //队尾
}SqQueue;

/* 初始化队列 */
Status InitQueue(SqQueue *Q)
{
    Q->base = (ElemType *) malloc(sizeof(ElemType) * MAXSIZE);
    if(!Q->base)
        exit(OVERFLOW);
    Q->rear = Q->front = 0;
    return OK;
}

/* 入队 */
Status EnQueue(SqQueue *Q, ElemType e)
{
    if( (Q->rear+1)%MAXSIZE == Q->front )  //队列已满
    {
        printf("Error:No room of the Queue!");
        exit(OVERFLOW);
    }
    Q->base[Q->rear] = e;
    Q->rear = (Q->rear+1) % MAXSIZE;
    return OK;
}

/* 出队 */
Status DeQueue(SqQueue *Q, ElemType *e)
{
    if( Q->front == Q->rear )  //队列为空
    {
        printf("Error: Empty Queue!");
        exit(OVERFLOW);
    }
    *e = Q->base[Q->front];
    Q->front = (Q->front+1) % MAXSIZE;
    return OK;
}

/* 判断队列是否为空 */
Status IsEmpty(SqQueue Q)
{
    return (Q.front == Q.rear);
}

/* 获取队列的长度 */
int GetLength(SqQueue Q)
{
    int n = 0;
    while( Q.front != Q.rear )
    {
        n++;
        Q.front = (Q.front+1) % MAXSIZE;
    }
    return n;
}

/* 获取队头元素 */
Status GetHead(SqQueue Q, ElemType *e)
{
    if(Q.front != Q.rear)
    {
        *e = Q.base[Q.front];
        return OK;
    }
    printf("Warning:Try to get head element from empty queue!\n");
    return ERROR;
}

/* 遍历队列 */
Status TraverseQueue(SqQueue Q)
{
    while(Q.front != Q.rear)
    {
        printf("%d ", Q.base[Q.front]);
        Q.front = (Q.front+1) % MAXSIZE;
    }
    printf("\n");
}

/* 销毁队列 */
Status DestroyQueue(SqQueue *Q)
{
    Q->front = Q->rear = 0;
    free(Q->base);
}

/* 清空队列 */
Status ClearQueue(SqQueue *Q)
{
    memset(Q->base, 0, sizeof(ElemType)*MAXSIZE);
    Q->front = Q->rear = 0;
    return OK;
}

int main()
{
    SqQueue Q;
    int e;
    if( InitQueue(&Q) )
    {
        for(int i=0; i<10; i++)
        {
            EnQueue(&Q, i+1);
        }
        TraverseQueue(Q);
        printf("Length:%d\n", GetLength(Q));
        GetHead(Q, &e);
        printf("Head:%d\n", e);

        DeQueue(&Q, &e);
        printf("Out:%d\n", e);
        TraverseQueue(Q);
    }
    
}

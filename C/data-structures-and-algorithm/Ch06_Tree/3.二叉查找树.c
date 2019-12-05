#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define OK 1
#define ERROR 0

typedef int Status;
typedef int ElemType;

//存储结构
typedef struct BiTNode
{
    ElemType data; 
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

BiTree FindMin(BiTree T);

/*初始化
Status Init(BiTNode **T)
{
    *T = (BiTNode *)malloc(sizeof(BiTNode));
    if(!(*T))
        exit(OVERFLOW);
    (*T)->lchild = NULL;
    (*T)->rchild = NULL;
    return OK;
}                                
*/

//插入
BiTree Insert(ElemType e, BiTree T)
{
    if( T == NULL )
    {
        T = (BiTNode *)malloc(sizeof(BiTNode));
        if( !T )
            exit(OVERFLOW);
        T->data = e;
        T->lchild = T->rchild = NULL;
    }
    else if(e < T->data)
    {
        T->lchild = Insert(e, T->lchild);
    }
    else if(e > T->data)
    {
        T->rchild = Insert(e, T->rchild);
    }

    return T;   //隐含了 e == T->data 的情况
}

//delete node by value
BiTree Delete(ElemType e, BiTree T)
{
    BiTree temp;
    if(T == NULL)
        exit(OVERFLOW);
    else if(e < T->data)
    {    
        T->lchild = Delete(e, T->lchild);
    }
    else if(e > T->data)
    {
        T->rchild = Delete(e, T->rchild);
    }
    else if( T->lchild && T->rchild )  // 2 children
    {
        temp = FindMin(T->rchild);  //因为右边最小值无左结点，问题转化为0或1个结点
        T->data = temp->data;
        T->rchild = Delete(T->data, T->rchild); 
    }
    else  // 0 or 1 children
    {
        temp = T;
        if(T->lchild == NULL)
            T = T->rchild;
        else if(T->rchild == NULL)
            T = T->lchild;
        free(temp);
    }
    return T;
}

//根据元素查找
BiTree FindElem(ElemType e, BiTree T)
{
    if(T == NULL)
        return NULL;
    else if(e < T->data)
    {
        return FindElem(e, T->lchild);
    }
    else if(e > T->data)
    {
        return FindElem(e, T->rchild);
    }
    else           // e == T->data
        return T;
}

//查找最大值
BiTree FindMax(BiTree T)
{
    if(T == NULL)
        return NULL;
    else if(T->rchild == NULL)
        return T;
    return FindMax(T->rchild);
}

//查找最小值
BiTree FindMin(BiTree T)
{
    if(T == NULL)
        return NULL;
    while(T->lchild)
    {   
        T = T->lchild; 
    }
    return T;
}

//前序遍历
Status PreOrderTraverse(BiTree T)
{
    if (T)
    {
        printf("%d ", T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
    return OK;
}

/*
            50
        25       56
     12    31        64
*/
int main()
{
    BiTree T;
    
    T = Insert(50,T);
    T = Insert(25,T);
    T = Insert(56,T);
    T = Insert(12,T);
    T = Insert(31,T);
    T = Insert(64,T);

    PreOrderTraverse(T);
    printf("\n");

    printf("Min:%d\n", FindMin(T)->data);
    printf("Max:%d\n", FindMax(T)->data);

    //以下两句会报段错误，为什么？
    //BiTree tmp = FindElem(31, T);
    //printf("Find:%d\n", tmp->data);
    printf("Find:%d\n", FindElem(31, T)->data);

    T = Delete(25, T);
    PreOrderTraverse(T);

    return 0;
}


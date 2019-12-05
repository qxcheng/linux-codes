#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define OK 1
#define ERROR 0

typedef int Status;
typedef int ElemType;

typedef struct HeapStruct
{
    int capacity;
    int size;
    ElemType *elements;
}HeapStruct, *PriorityQueue;



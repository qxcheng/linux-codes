#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 100

typedef struct node
{
    int data;
    struct node *next;
}LNode, *LinkList;


/* calculate hash value */
int hashfun(int i)
{
    return i%SIZE;
}

/* create a hashtable */
void create(LinkList array[])
{

    int data;
    scanf("%d",&data);
    while(data > 0)
    {
        LinkList temp = (LinkList) malloc(sizeof(LNode));
        if(!temp)
            return ;
        temp->data = data;
        int index = hashfun(data);
        temp->next = array[index];
        array[index] = temp;
        scanf("%d", &data);
    }
}

void traverse(LinkList array[])
{
    for(int i=0; i<SIZE; i++)
    {
        if(array[i])
        {
            LinkList temp = array[i];
            while(temp)
            {
                printf("address %d : %d\n",i,temp->data);
                temp = temp->next;
            }
        }
    }
}

void insert(LinkList array[], int data)
{
    int index = hashfun(data);
    LinkList temp = (LinkList) malloc(sizeof(LNode));
    if(!temp)
        return ;
    temp->data = data;
    temp->next = array[index];
    array[index] = temp;
}

void delete(LinkList array[], int data)
{
    int index = hashfun(data);
    if(array[index])
    {
        LinkList temp = array[index];
        while(temp)
        {
            if(temp->data == data)
                break;
            temp = temp->next;
        }
        if(!temp)
        {
            printf("data not exist 2!\n");
            return ;
        }
        else
        {
            LinkList p = array[index];
            if(p == temp)
            {
                array[index] = temp->next;
                free(temp);
            }
            else
            {
                while(p->next != temp)
                    p = p->next;
                p->next = temp->next;
                free(temp);
            }
        }
    }
    else
    {
        printf("data not exist!\n");
        return ;
    }
}

void find(LinkList array[], int data)
{
    int index = hashfun(data);
    if(array[index])
    {
        LinkList temp = array[index];
        while(temp)
        {
            if(temp->data == data)
            {
                printf("address : %d data : %d\n",index,temp->data);
                return ;
            }
            temp = temp->next;
        }
        printf("data not exist 2\n");
    }
    else
    {
        printf("data not exist\n");
        return ;
    }
}

int main()
{
    LinkList hashtable[SIZE];
    for(int i=0; i<SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    create(hashtable); //1 2 3 4 5 101 102 103 
    traverse(hashtable);
    printf("\n");

    insert(hashtable,201);
    insert(hashtable,6);
    traverse(hashtable);
    printf("\n");

    delete(hashtable, 7);
    delete(hashtable, 301);
    delete(hashtable, 201);
    traverse(hashtable);
    printf("\n");

    find(hashtable, 8);
    find(hashtable, 401);
    find(hashtable, 103);
    printf("\n");

    return 0;
}


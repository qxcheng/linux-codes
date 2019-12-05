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

Status init(LinkList *L)
{
    *L = (LinkList) malloc(sizeof(LNode));
    if (!L)
        return ERROR;
    (*L)->next = NULL;
    return OK;
}

Status insert(LinkList L, int i, Elemtype e)
{
    LinkList s, p = L;
    int j = 1;
    while (p && j<i)
    {
        j++;
        p = p->next;
    }
    if (!p || j>i)
        return ERROR;
    s = (LinkList) malloc(sizeof(LNode));
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

Status del(LinkList L, int i, Elemtype *e)
{
    LinkList s, p = L;
    int j = 1;
    while (p->next && j<i)
    {
        j++;
        p = p->next;
    }
    if (!p->next || j>i)
        return ERROR;
    s = p->next;
    *e = s->data;
    p->next = s->next;
    free(s);
    return OK;
}

//get an elem by position
Status get(LinkList L, int i, Elemtype *e)
{
    LinkList p = L;
    int j = 0;
    while (p && j<i)
    {
        j++;
        p = p->next;
    }
    if (!p || j>i)
        return ERROR;
    *e = p->data;
    return OK;
}

Status traverse(LinkList L)
{
    LinkList p = L->next;
    while(p)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
    return OK;
}

int len(LinkList L)
{
    LinkList p = L->next;
    int n=0;
    while(p)
    {
        n++;
        p = p->next;
    }
    return n;
}

Status clear(LinkList L)
{
    LinkList s, p = L->next;
    while(p)
    {
        s = p;
        p = p->next;
        free(s);
    }
    L->next = NULL;
    return OK;
}

Status destroy(LinkList *L)
{
    clear(*L);
    free(*L);
    return OK;
}

int main()
{
    LinkList L;
    init(&L);
    for (int i=1; i<10; i++)
        insert(&L, i, i);
    Elemtype e;
    del(&L, 5, &e);
    printf("%d\n", e);
    get(L, 6, &e);
    printf("%d", e);
    return 0;
}


#if 0  // by me

#include<stdio.h>
#include<stdlib.h>

typedef struct node{
    int num;
    struct node *next;
}LNode, *LinkList;

void create(LinkList *L);
void traverse(LinkList L);
int len(LinkList L);
LinkList find_val(LinkList L, int num);
void find_pos(LinkList L, int n);
void insert(LinkList L, int n, int num);
void del_val(LinkList L, int num);
void del_pos(LinkList L, int n);
void sort(LinkList L);
void re_traverse(LinkList L);
void de_repeat(LinkList L);
LinkList join(LinkList, LinkList);

void create(LinkList *L)
{
    *L = (LinkList) malloc(sizeof(LNode));
    (*L)->next = NULL;

    char key;
    printf("whether to add new node?y/n\n");
    scanf("%c", &key);

    while(key == 'y')
    {
        int num;
        printf("input a num:\n");
        scanf("%d", &num);

        LinkList temp = (LinkList) malloc(sizeof(LNode));
        temp->num = num;
        temp->next = (*L)->next;
        (*L)->next = temp;
        printf("whether to add new node?y/n\n");
        getchar();
        scanf("%c", &key);
    }
    return ;
}

void traverse(LinkList L)
{
    LinkList temp = L->next;
    while(temp)
    {
        printf("%d ", temp->num);
        temp = temp->next;
    }
    printf("\n");
    return ;
}

int len(LinkList L)
{
    LinkList temp = L->next;
    int n=0;
    while(temp)
    {
        n++;
        temp = temp->next;
    }
    return n;
}

LinkList find_val(LinkList L, int num)
{
    LinkList temp = L->next;
    int n=1;
    while(temp)
    {
        if(temp->num == num)
        {
            printf("The node is %dth!\n",n);
            return temp;
        }
        temp = temp->next;
        n++;
    }
    printf("Fail to find the node!\n");
    return NULL;
}

void find_pos(LinkList L, int n)
{
    if(n > len(L))
    {
        printf("The node doesn't exist!\n");
        return ;
    }
    LinkList temp = L;
    for(int i=0; i<n; i++)
    {
        temp = temp->next;
    }
    printf("%d\n", temp->num);
}

void insert(LinkList L, int n, int num)
{
    if(n<1 || n>len(L)+1)
    {
        printf("Wrong position to insert!\n");
        return ;
    }
    LinkList temp = L;
    for(int i=0; i<n-1; i++)
    {
        temp = temp->next;
    }
    LinkList s = (LinkList) malloc(sizeof(LNode));
    s->num = num;
    s->next = temp->next;
    temp->next = s;
    printf("Succeed to insert!\n");
    return ;
}

void del_val(LinkList L, int num)
{
    LinkList temp = L->next;
    LinkList s = L;
    while(temp)
    {
        if(temp->num == num)
        {
            s->next = temp->next;
            free(temp);
            printf("Successful to delete!\n");
            return ;
        }
        temp = temp->next;
        s = s->next;
    }
    printf("Fail to find the node!\n");
    return ;
}

void del_pos(LinkList L, int n)
{
    if(n<1 || n>len(L))
    {
        printf("The node doesn't exist!\n");
        return ;
    }
    LinkList temp = L;
    for(int i=0; i<n-1; i++)
    {
        temp = temp->next;
    }
    LinkList s = temp->next;
    temp->next = s->next;
    free(s);
    printf("Succeed to delete the node!\n");
    return ;
}

LinkList find_max(LinkList L)
{
    LinkList temp = L->next;
    LinkList pmax = L->next;
    while(temp)
    {
        if(temp->num > pmax->num)
        {
            pmax = temp;
        }
        temp = temp->next;
    }
    return pmax;
}

void sort(LinkList L)
{
    LinkList head = NULL;
    
    LinkList p = L->next;
    while(p)
    {
        LinkList pmax = find_max(L);
        
        LinkList temp = (LinkList) malloc(sizeof(LNode));
        temp->num = pmax->num;
        temp->next = head;
        head = temp;
        
        LinkList s = L;
        while(s->next != pmax)
        {
            s = s->next;
        }
        s->next = pmax->next;
        free(pmax);
        p = L->next;
    }
    L->next = head;
    return ;
}

void re_traverse(LinkList L)
{
    LinkList temp = L;
    if(temp == NULL)
    {
        return ;
    }
    else
    {
        re_traverse(temp->next);
        printf("%d ", temp->num);
    }
}

void de_repeat(LinkList L)
{
    sort(L);
    LinkList temp = L->next;
    while(temp && temp->next)
    {
        if(temp->num == temp->next->num)
        {
            LinkList s = temp->next;
            temp->next = s->next;
            free(s);
            continue;
        }
        else
        {
            temp = temp->next;
        }
    }
    return ;
}

LinkList join(LinkList a, LinkList b)
{
    LinkList L = (LinkList) malloc(sizeof(LNode));
    L->next = NULL;

    LinkList pa = a->next;
    LinkList pb = b->next;

    while(pa && pb)
    {
        if(pa->num < pb->num)
        {
            LinkList p = pa->next;
            pa->next = L->next;
            L->next = pa;
            pa = p;
        }
        else
        {
            LinkList s = pb->next;
            pb->next = L->next;
            L->next = pb;
            pb = s;
        }
    }
    while(pa)
    {
        LinkList x = pa->next;
        pa->next = L->next;
        L->next = pa;
        pa = x;
    }
    while(pb)
    {
        LinkList y = pb->next;
        pb->next = L->next;
        L->next = pb;
        pb = y;
    }
    return L;
}

void menu(LinkList L)
{
    printf("1:traverse\n");
    printf("2:length\n");
    printf("3:insert\n");
    printf("4:find by value\n");
    printf("5:find by position\n");
    printf("6:delete by value\n");
    printf("7:delete by position\n");
    printf("8:sort\n");
    printf("9:retraverse\n");
    printf("10:remove repeated elem\n");
    int key;
    printf("\nselect your operate:");
    scanf("%d", &key);
    

    int n, num;
    LinkList p;
    switch(key)
    {
        case 1:
            traverse(L);
            getchar();
            getchar();
            menu(L);
            break;
        case 2:
            printf("length=%d\n", len(L));
            getchar();
            getchar();
            menu(L);
            break;
        case 3:
            printf("input a number:");
            scanf("%d", &num);
            printf("input a position:");
            scanf("%d", &n);
            insert(L,n,num);
            getchar();
            getchar();
            menu(L);
            break;
        case 4:
            printf("input a number:");
            scanf("%d", &num);
            p = find_val(L, num);
          //  printf("%d\n",p->num);
            getchar();
            getchar();
            menu(L);
            break;
        case 5:
            printf("input a position:");
            scanf("%d", &n);
            find_pos(L, n);
            getchar();
            getchar();
            menu(L);
            break;
        case 6:
            printf("input a number:");
            scanf("%d", &num);
            del_val(L, num);
            getchar();
            getchar();
            menu(L);
            break;
        case 7:
            printf("input a position:");
            scanf("%d", &n);
            del_pos(L, n);
            getchar();
            getchar();
            menu(L);
            break;
        case 8:
            sort(L);
            printf("Succeed to sort!\n");
            getchar();
            getchar();
            menu(L);
            break;
        case 9:
            re_traverse(L->next);
            getchar();
            getchar();
            menu(L);
            break;
        case 10:
            de_repeat(L);
            getchar();
            getchar();
            menu(L);
            break;
    }
    
}

int main()
{
    LinkList L;
    create(&L);
    menu(L);

#if 0
    /* A test of join two sorted linklists! */

    LinkList a,b;
    create(&a);
    getchar();
    create(&b);
    sort(a);
    sort(b);
    LinkList L = join(a,b);
    traverse(L);
#endif

    return 0;

}
#endif
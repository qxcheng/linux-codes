#include <iostream>
#include "linklist.h"

using namespace std;

//LinkNode
template <typename Type>
LinkNode<Type>::LinkNode()
{
    data = 0;
    next = NULL;
}

template <typename Type>
LinkNode<Type>::LinkNode(const Type &item)
{
    data = item;
    next = NULL;
}

/*
template <typename Type>
LinkNode<Type>::LinkNode(const LinkNode<Type> &obj)
{
    
}

template <typename Type>
LinkNode<Type>::~LinkNode()
{
    
}


template <typename Type>
LinkNode<Type>* & LinkNode<Type>::operator=(const LinkNode<Type>* &pNode)
{
    
}
*/


//LinkList
template <typename Type>
LinkList<Type>::LinkList()
{
    this->len = 0;
    this->pHead = NULL;
}

template <typename Type>
LinkList<Type>::LinkList(const LinkList<Type> &obj)
{
    
}

template <typename Type>
LinkList<Type>::~LinkList()
{
    LinkNode<Type> *temp = this->pHead;
    LinkNode<Type> *t = NULL;
    while(temp)
    {
        t = temp;
        temp = temp->next;
        delete t;
    }
}


template <typename Type>
void LinkList<Type>::insert(Type data)
{
    LinkNode<Type> *temp = new LinkNode<Type>;
    temp->data = data;
    temp->next = this->pHead;
    this->pHead = temp;
    this->len++;
}

template <typename Type>
void LinkList<Type>::del()
{
    LinkNode<Type> *temp = this->pHead;
    if(temp)
    {
        this->pHead = this->pHead->next;
    }
    delete temp;
    this->len--;
}

template <typename Type>
int  LinkList<Type>::length()
{
    return this->len;    
}

template <typename Type>
void LinkList<Type>::traverse()
{
    LinkNode<Type> *temp = this->pHead;
    while(temp)
    {
        cout << temp->data << " " ;
        temp = temp->next;
    }
    cout << endl;
}

#pragma once

template <typename Type>
class LinkNode
{
public:
    Type data;
    LinkNode *next;

public:
    LinkNode();
    LinkNode(const Type &item);
   // LinkNode(const LinkNode& obj);
   // ~LinkNode();

public:
   // LinkNode* & operator=(const LinkNode* &pNode);

};

template <typename Type>
class LinkList
{
public:
    LinkList();
    LinkList(const LinkList& obj);
    ~LinkList();

public:
    void insert(Type data);
    void del();
    int  length();
    void traverse();

private:
    int len;
    LinkNode<Type> *pHead;
};

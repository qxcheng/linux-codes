#include <iostream>
#include <cstring>
#include <cstdlib>
#include "mystring.h"


//1 initial
MyString::MyString()
{
    m_len = 0;
    m_p = NULL;
}

MyString::MyString(const char *p)
{
    m_len = strlen(p);
    m_p = new char[m_len+1];
    memset(m_p, m_len+1, 0);
    strcpy(m_p, p);
}

MyString::MyString(const MyString &obj)
{
    m_len = obj.m_len;
    m_p = new char[m_len+1];
    memset(m_p, m_len+1, 0);
    strcpy(m_p, obj.m_p);
}

MyString::~MyString()
{
    if(m_p != NULL)
    {
        delete [] m_p;
        m_len = 0;
        m_p = NULL;
    }
}

//2 function
int MyString::length()
{
    return m_len;
}

MyString& MyString::operator=(const char *p)
{
    if(m_p != NULL)
    {
        delete [] m_p;
        m_len = 0;
        m_p = NULL;
    }
    m_len = strlen(p);
    m_p = new char[m_len+1];
    memset(m_p, m_len+1, 0);
    strcpy(m_p, p);
}

MyString& MyString::operator=(const MyString &s)
{
    if(m_p != NULL)
    {
        delete [] m_p;
        m_len = 0;
        m_p = NULL;
    }
    m_len = s.m_len;
    m_p = new char[m_len+1];
    memset(m_p, m_len+1, 0);
    strcpy(m_p, s.m_p);
}

char& MyString::operator[](int index)
{
    return m_p[index];
}

std::ostream& operator<<(std::ostream &out, const MyString &s)
{
    out << s.m_p;
    return out;
}





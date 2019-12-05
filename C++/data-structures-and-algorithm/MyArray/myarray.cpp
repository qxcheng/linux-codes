#include <iostream>
#include "myarray.h"

using namespace std;

//init copy delete
template <typename T>
MyArray<T>::MyArray(int length)
{
    m_length = length;
    m_space = new T[m_length];
}

template <typename T>
MyArray<T>::MyArray(const MyArray<T> &obj)
{
    this->m_length = obj.m_length;
    this->m_space = new T[this->m_length];
    for(int i=0; i<this->m_length; i++)
    {
        this->m_space[i] = obj.m_space[i];   
    }
}

template <typename T>
MyArray<T>::~MyArray()
{
    m_length = 0;
    if(this->m_space != NULL)
    {
        delete [] this->m_space;
    }
}

//operator overload
template <typename T>
T& MyArray<T>::operator[](int index)
{
    return this->m_space[index];
}

template <typename T>
MyArray<T>& MyArray<T>::operator=(const MyArray<T> &obj)
{
    if(this->m_space != NULL)
    {
        delete []  m_space;
    }
    this->m_length = obj.m_length;
    this->m_space = new T[this->m_length];
    for(int i=0; i<this->m_length; i++)
    {
        this->m_space[i] = obj.m_space[i];
    }
}

template <typename T>
ostream& operator<<(ostream &out, MyArray<T> &obj)
{
    for(int i=0; i<obj.m_length; i++)
    {
        out << obj[i] << " ";
    }
    return out;
}

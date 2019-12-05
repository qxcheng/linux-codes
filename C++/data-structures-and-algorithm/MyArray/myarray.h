#pragma once

template <typename T>
class MyArray
{

    public:
        MyArray(int length);
        MyArray(const MyArray &obj);
        ~MyArray();

        T& operator[](int index);
        MyArray& operator=(const MyArray &obj);

        template <typename T1>
        friend std::ostream& operator<<(std::ostream &out, MyArray<T1> &obj);
    
    private:
        T   *m_space;

    public:
        int m_length;
};



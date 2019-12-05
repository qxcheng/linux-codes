#pragma once


class MyString
{
public:
    MyString();
    MyString(const char *p);
    MyString(const MyString &obj);
    ~MyString();

public:
    int length();


public:
    MyString& operator=(const char *p);
    MyString& operator=(const MyString &s);

    char& operator[](int index);

    friend std::ostream& operator<<(std::ostream &out, const MyString &s);




private:
    int  m_len;
    char *m_p;

};


#include <iostream>
#include "mystring.h"

using namespace std;

int main()
{ 
    MyString s1, s4;
    MyString s2 = "Hello";
    cout << s2 << " "<< s2[0] << endl;

    MyString s3 = s2;
    cout << s3 << endl;

    s1 = "s1oop";
    s4 = s1;
    s2 = s1;

    cout << "s1:"<<s1 <<" s2:"<<s2<< " s4:"<<s4<<endl;


    return 0;
}



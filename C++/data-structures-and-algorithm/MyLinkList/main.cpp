#include <iostream>
#include "linklist.cpp"

using namespace std;

void objplay()
{
    
}

int main()
{
    LinkList<int> l1;

    for(int i=0; i<10; i++)
    {    
        l1.insert(i+1);
    }

    cout << "length: " << l1.length() << endl;
    l1.traverse();

    l1.del();
    cout << "length: " << l1.length() << endl;
    l1.traverse();


}

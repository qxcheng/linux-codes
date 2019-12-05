#include "iostream"
#include "myarray.cpp"

using namespace std;

int main()
{
    MyArray<float> a1(10);
#if 1
    for(int i=0; i<10; i++)
    {
        a1[i] = i+1.1;
    }
    cout << a1 << endl;

    MyArray<float> a2 = a1;
    cout << a2 << endl;

    MyArray<float> a3(10);
    a3 = a2;
    cout << a3 << endl;
#endif
    return 0;
}

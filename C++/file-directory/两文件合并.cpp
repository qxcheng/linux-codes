#include<iostream>
#include<fstream>
using namespace std;

int main()
{
	int i=1;
	char c[1000];
	ifstream ifile1("D:\\1.cpp");
	ifstream ifile2("D:\\2.cpp");
	ofstream ofile("D:\\3.cpp");
	while(!ifile1.eof())
	{
		ifile1.getline(c,999);
		ofile<<c<<endl;
	}
	while(!ifile2.eof())
	{
		ifile2.getline(c,999);
		ofile<<c<<endl;
	}
	ifile1.close();
	ifile2.close();
	ofile.close();
	return 0;
}
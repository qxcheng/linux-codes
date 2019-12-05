#include<iostream>
#include<fstream>
using namespace std;
int main()
{
	int i=1;
	char c[1000];
	ifstream ifile("D:\\1.cpp");
	ofstream ofile("D:\\2.cpp");
	while(!ifile.eof())
	{
		ofile<<i++<<": ";
		ifile.getline(c,999);
		ofile<<c<<endl;
	}
	ifile.close();
	ofile.close();
	return 0;
}
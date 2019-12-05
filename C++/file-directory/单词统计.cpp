#include<iostream>
#include<fstream>
using namespace std;
bool isalph(char c)
{
	return ((c>='A' && c<='Z') || (c>='a' && c<='z'));
}
int main()
{
	ifstream ifile("C:\\daily.doc");
	char text[1000];
	bool inword=false;
	int rows=0,words=0;
	int i;
	while(!ifile.eof())
	{
		ifile.getline(text,999);
		rows++;
		i=0;
		while(text[i]!=0)
		{
			if(!isalph(text[i]))
				inword=false;
			else if(isalph(text[i]) && inword==false)
			{
				words++;
				inword=true;
			}
			i++;
		}
	}
	cout<<"rows= "<<rows<<endl;
	cout<<"words= "<<words<<endl;
	ifile.close ();
	return 0;
}
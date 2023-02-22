#include<iostream>

using namespace std;
int main()
{
	extern char** environ;
	for(int i=0;environ[i]!=NULL;i++)
	{
		cout<<environ[i]<<endl;
	}
	return 0;
}

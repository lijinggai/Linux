#include<iostream>
#include<unistd.h>
using std::cout;
using std::endl;
//using namespace std;

int main()
{
	int tmp=10;
	if(fork()==0)
	{
		tmp=20;
		cout<<"tmp:"<<tmp<<"  addr:"<<&tmp<<endl;
	}
	else
	{
		cout<<"tmp:"<<tmp<<"  addr:"<<&tmp<<endl;
	}
	return 0;
}

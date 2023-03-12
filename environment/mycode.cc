#include<string.h>
//#include<unistd.h>
#include<stdlib.h>
#include<iostream>
using std::cout;
using std::endl;

int main(int argc,char* argv[])
{
	cout<<getenv("MYENV")<<endl;
	return 0;
//	for(int i=0;i<argc;i++)
//	{
//		cout<<argv[i]<<endl;
//	}
//	return 0;
//	if(argc!=2)
//	{
//		cout<<"mycode -a/-b"<<endl;
//		return 1;
//	}
//	if(strcmp(argv[1],"-a")==0)
//	{
//		cout<<"Option 1"<<endl;
//	}
//	else if(strcmp(argv[1],"-b")==0)
//	{
//		cout<<"Option 2"<<endl;
//	}
//	else
//	{
//		cout<<"default"<<endl;
//	}
//	return 0;
}


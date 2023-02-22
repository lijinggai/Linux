#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;

int main()
{
	cout<<"command begin."<<endl;
	//char* argv[]={"ls","-a","-l",NULL};
	char* env[]={"hello wolrd","hello world",NULL};
	execle("./printfenv"/*ÎÄ¼þÃû*/,"./printfenv",NULL,env);
	//execv("/usr/bin/ls",argv);
	cout<<"command end."<<endl;
//	if(fork()==0)
//	{
//		cout<<"I am child!"<<endl;
//		execl()
//	}
	return 0;
}

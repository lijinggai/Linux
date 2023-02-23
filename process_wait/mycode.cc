#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
using std::cout;
using std::endl;
//void func(int* tmp)
//{	
//	*tmp=*tmp/0;
//}
int main()
{
	int pid=fork();
	if(pid==0)
	{
		for(int i=0;i<5;i++)
		{
			cout<<"I am child "<<getpid()<<endl;
			sleep(1);
		}
		exit(20);
	}
	else
	{
		cout<<"wait begin!"<<endl;
		int status=0;
		while(1)
		{
			int ret=waitpid(pid,&status,WNOHANG);
			if(ret==0)
			{
				cout<<"I'm woking."<<endl;
			}
			else if(ret>0)
			{
				cout<<"wait success!"<<ret<<endl;
				if(WIFEXITED(status))
					cout<<"exit code:"<<WEXITSTATUS(status)<<endl;
				break;
				//cout<<"exit status:"<<((status>>8)&0xFF)<<" exit signal:"<<(status&0x7F)<<endl;
			}
			else{
				cout<<"wait fail!"<<ret<<endl;
				break;
			}
			sleep(1);
		}
	}












//	int tmp=10;
//	func(&tmp);
//	cout<<"hello world";
//	return 0;
//	for(int i=0;i<140;i++)
//	{
//		cout<<strerror(i)<<endl;
//	}
//	return 0;
//	if(fork()==0)
//	{
//		cout<<"child:"<<"I am child"<<endl;
//	}
//	else
//	{
//		cout<<"parent:"<<"I am parent"<<endl;
//	}
	return 0;
}

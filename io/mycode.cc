#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;

int main()
{
	int fd=open("test.txt",O_WRONLY|O_TRUNC|O_CREAT);
	if(fd<0)
	{
		cerr<<"opne fail";
		return 1;
	}
	dup2(fd,1);
	cout<<"hello wrold"<<endl;
	cout<<"hello IO"<<endl;
//	close(0);
//	int fd=open("./test.txt",O_RDONLY);
//	if(fd<0)
//	{
//		cerr<<"open fail";
//		return(1);
//	}
//	char s[50];
//	while(fgets(s,sizeof(s)-1,stdin))
//		cout<<s;
//	close(1);
//	int fd1=open("test.txt",O_WRONLY|O_CREAT|O_APPEND,0644/*八进制给初始值*/);
//	if(fd1<0)
//	{
//		cerr<<"open fail";
//		return 1;
//	}
//	
	//int fd2=open("test.txt1",O_WRONLY|O_CREAT,0644/*八进制给初始值*/);
	//int fd3=open("test.txt2",O_WRONLY|O_CREAT,0644/*八进制给初始值*/);
	//int fd4=open("test.txt3",O_WRONLY|O_CREAT,0644/*八进制给初始值*/);
	//printf("%d %d %d %d\n",fd1,fd2,fd3,fd4);
	return 0;
}

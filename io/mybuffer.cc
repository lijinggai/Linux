#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<string.h>
using namespace std;
int main()
{
	const char* s1="hello buffer\n";
	write(1,s1,strlen(s1));//ϵͳ�ӿ�
	//c���Խӿ�
	printf("hello world\n");
	fprintf(stdout,"hello world\n");
	fork();
	//close(1);
	return 0;
}

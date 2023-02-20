#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int uninit;
int init=100;
int main()
{
	printf("code addr:%p\n",main);
	const char* s="hello world!";
	printf("string addr:%p\n",s);
	printf("uninit addr:%p\n",&uninit);
	printf("init addr:%p\n",&init);
	const int* ph=(int*)malloc(40);
	const int* ph1=(int*)malloc(20);
	printf("heap addr1:%p\n",ph);
	printf("heap addr2:%p\n",ph1);
	printf("stack addr1:%p\n",&ph);
	printf("stack addr2:%p\n",&ph1);
	
	return 0;
}

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	int pipefd[2]={0};
	if(pipe(pipefd)!=0)
	{
		perror("pipe error!\n");
		exit(1);
	}
	if(fork()==0)
	{
		close(pipefd[0]);
		const char* message="hello world\n";
		while(1)
		{
			write(pipefd[1],message,strlen(message));
			sleep(1);
		}
		exit(1);
	}
	close(pipefd[1]);
	while(1)
	{
		char* buffer[64];
		ssize_t s=read(pipefd[0],buffer,sizeof(buffer)-1);
		if(s==0)
		{
			printf("子进程关闭写入\n");
			break;
		}
		else if(s>0)
		{
			buffer[s]=0;
			printf("child say to father:%s",buffer);
		}
		else
		{
			printf("读取失败\n");
			break;	
		}
	}
	return 0;
}

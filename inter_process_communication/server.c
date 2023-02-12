#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define MYFIFO "./myfifo"
int main()
{
	umask(0);
	if(mkfifo(MYFIFO,0666)<0)
	{
		perror("mkfifo fail\n");
		exit(1);
	}
	int fd = open("myfifo",O_RDONLY);
	if(fd<0)
	{
		perror("open fail\n");
		exit(2);
	}
	while(1)
	{
		char buffer[64];
		ssize_t s=read(fd,buffer,63);
		if(s>0)
		{
			buffer[s]=0;
			if(strcmp(buffer,"ls")==0)
			{
				execlp("ls","ls","-l",NULL);
				return(1);
			}
			printf("client say to server:%s\n",buffer);
		}
		else if(s==0)
		{
			printf("client cliose\n");
			break;
		}
		else
		{
			printf("read fail");
			break;
		}
	}
	return 0;
}

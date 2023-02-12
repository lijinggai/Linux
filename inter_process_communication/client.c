#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#define MYFIFO "./myfifo" 

int main()
{
	int fd=open(MYFIFO,O_WRONLY);
	if(fd<0)
	{
		perror("open\n");
		return 1;
	}
	while(1)
	{
		char message[64];
		printf("please enter:");
		fflush(stdout);
		ssize_t s=read(0,message,63);
		message[s]=0;
		write(fd,message,strlen(message)-1);
	}
	return 0;
}


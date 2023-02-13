#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#define PATHNAME "./"
#define PROJID 0x5555

int main()
{
	key_t key=ftok(PATHNAME,PROJID);
	int shmid=shmget(key,4096,IPC_CREAT|0666);
	char* mem=shmat(shmid,NULL,0);
	printf("client process attaches success\n");
	while(1)
	{
		printf("%s\n",mem);
		sleep(1);
	}
	shmdt(mem);
	return 0;
}

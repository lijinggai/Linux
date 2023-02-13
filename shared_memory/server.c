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
	if(shmid<0)
	{
		perror("shmget\n");
		return 1;
	}
	//printf("key:%d,shmid:%d\n",key,shmid);
	char* mem=(char*)shmat(shmid,NULL,0);
	printf("server process attaches success\n");
	//sleep(6);
	char i='a';
	while(i<='z')
	{
		mem[i-'a']=i;
		i++;
		mem[i-'a']=0;
		sleep(2);
	}

	shmdt(mem);
	printf("server process detach success\n");
	shmctl(shmid,IPC_RMID,NULL);
	printf("shared memory close\n");
	return 0;
}

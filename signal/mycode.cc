#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<iostream>
using namespace std;
void handler(int signo)
{
	cout<<signo<<endl;
	cout<<getpid()<<endl;
}
int main()
{
	signal(SIGCHLD,handler);
	if(fork()==0)
	{
		int cnt=5;
		while(cnt)
		{
			cout<<"I am child process, "<<getpid()<<endl;
			cnt--;
			sleep(1);
		}
		return 0;
	}
	while(1);
}
// int main()
// {
// 	const int t=10;
// 	int* p=const_cast<int*>(&t);
// 	*p=20;
// 	printf("t: %d\n",t);
// 	printf("*p: %d\n",*p);
// 	return 0;
// }
// void show(sigset_t* set)
// {
// 	int i=1;
// 	while(i<32)
// 	{
// 		if(sigismember(set,i))
// 			printf("1");
// 		else
// 			printf("0");
// 		i++;
// 	}
// 	printf("\n");
// }
// int main()
// {
// 	sigset_t iset,pending;
// 	sigemptyset(&iset);

// 	sigaddset(&iset,2);
// 	sigprocmask(SIG_SETMASK,&iset,NULL);
// 	while(1){
// 		sigemptyset(&pending);
// 		sigpending(&pending);
// 		show(&pending);
// 		sleep(1);
// 	}
// 	return 0;
// }



//int count=0;
//void handler(int signo)
//{
//		printf("count: %d\n",count);
//}
//
//int main()
//{
//	alarm(1);
//	signal(14,handler);
//	while(1){
//		count++;
//	//	printf("count: %d\n",count);
//	}
//	return 0;
//}
//int main()
//{
//	int i=1;
//	while(i<=31){
//		signal(i,handler);
//		i++;
//	}
//	int tem=10;
//	tem/=0;
//	while(1)
//	{
//		printf("hello world  pid: %d\n",getpid());
//		sleep(1);
//	}
//	return 0;
//}

#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;

pthread_cond_t cond;//条件变量
pthread_mutex_t mtx;
void* Work(void* args)
{
    int num=*((int*)args);
    delete (int*)args;
    while(1)
    {
        pthread_cond_wait(&cond,&mtx);
        cout<<"worker: "<<num<<endl;
    }
}
void* Control(void* args)
{
    while(1)
    {
        pthread_cond_signal(&cond);
        sleep(1);
    }
}
int main()
{
    //初始化条件变量和互斥锁
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mtx,NULL);
    pthread_t master;
    pthread_t worker[5];

    for(int i=0;i<5;i++){//5个轻量级进程
        int* tmp=new int(i);
        pthread_create(worker+i,NULL,Work,(void*)tmp);
    }
    pthread_create(&master,NULL,Control,NULL);//使用同步管理另外5个轻量级进程按特定顺序执行

    //等待轻量级进程
    for(int i=0;i<5;i++)
    {
        pthread_join(worker[i],NULL);
    }
    //释放条件变量和互斥锁
    pthread_join(master,NULL);
    pthread_cond_destroy(&cond);
    return 0;
}
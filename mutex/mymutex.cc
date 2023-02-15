#include<iostream>
#include<cstdio>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t lk;//锁
int tickets=100;
void* thread_run(void* args)
{
    int tmp=*((int*)args);
    while(1)
    {
        pthread_mutex_lock(&lk);//加锁
        if(tickets>0)
        {
            usleep(10000);
            tickets--;
            printf("我是%d线程,还剩%d\n",tmp,tickets);
        }
        else
        {
            printf("没票了\n");
            pthread_mutex_unlock(&lk);//解锁
            break;
        }
        pthread_mutex_unlock(&lk);//解锁
    }
    return (void*)0;
}
int main()
{
    pthread_mutex_init(&lk,NULL);//初始化
    pthread_t tid[5];
    for(int i=0;i<5;i++)
    {
        int* t=new int(i);
        pthread_create(tid+i,NULL,thread_run,(void*)t);
    }
    for(int i=0;i<5;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&lk);//删除锁
    return 0;
}
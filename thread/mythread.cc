#include<iostream>
#include<cstdio>
#include<pthread.h>
#include<unistd.h>
using namespace std;

void* thread_run(void* msg)
{
    pthread_detach(pthread_self());//线程分离
    char* tmp=(char*)msg;
    //printf("%s : %lu\n",tmp,pthread_self());
    // while(1)
    // {
    //     //cout<<tmp<<pthread_self()<<endl;
    //     printf("%s : %lu\n",tmp,pthread_self());
    //     sleep(1);
    // }
    return (void*)520;
}
int main()
{
    pthread_t id;
    pthread_create(&id,NULL,thread_run,(void*)"I am thread");
    void* status=NULL;
    //pthread_cancel(id);//取消进程
    sleep(1);
    int ret=pthread_join(id,&status);
    printf("ret: %d,status: %d\n",ret,(long long)status);
    // while(1)
    // {
    //     //cout<<"id:"<<id<<endl;
    //     printf("id: %lu\n",id);
    //     //cout<<"I am mian thread :"<<pthread_self()<<endl;
    //     printf("I am mian thread : %lu\n",pthread_self());
    //     sleep(1);
    // }
    return 0;
}

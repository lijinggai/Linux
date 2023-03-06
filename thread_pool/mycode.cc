#include "thread_pool.hpp"
#include "Task.hpp"
#include<time.h>
#include<unistd.h>
#include<cstdlib>

using namespace ns_thread_pool;
using namespace ns_task;
int main()
{
    srand((long long)time(nullptr));
    thread_pool<Task>* tp=new thread_pool<Task>;
    tp->InitThreadPool();
    const char* algorithm="+-*/";

    while(true)
    {
        Task tmp(rand()%20+1,rand()%25,algorithm[rand()%4]);
        tp->Push(tmp);
        sleep(1);
    }

    return 0;
}
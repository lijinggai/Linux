#include "BlockQueue.hpp"
#include "Task.hpp"
#include<unistd.h>
#include<cstdlib>
#include<time.h>
using namespace ds_blockqueue;
using namespace ds_task;

const char* op="+-*/";
void* Consumer(void* args)
{
    BlockQueue<Task>* bq=(BlockQueue<Task>*)args;
    while(true)
    {
        Task t;
        int data=0;
        bq->Pop(&t);
        std::cout<<"消费数据：";
        t();
        sleep(1);
    }
}
void* Producer(void* args)
{
    BlockQueue<Task>* bq=(BlockQueue<Task>*)args;
    while(true)
    {

        int x=rand()%20+1;
        int y=rand()%20+1;
        Task t(x,y,op[rand()%4]);
        
        // //制造数据
        // int data=rand()%10+1;
        //std::cout<<"生产数据："<<data<<std::endl;
        std::cout<<"生产数据："<<std::endl;
        bq->Push(t);
        sleep(2);
    }
}
int main()
{
    srand((long long)time(nullptr));
    BlockQueue<Task>* bq=new BlockQueue<Task>;//创建阻塞队列
    pthread_t pid1;
    pthread_t pid2;
    //创建消费者和生产者
    pthread_create(&pid1,nullptr,Consumer,(void*)bq);
    pthread_create(&pid2,nullptr,Producer,(void*)bq);

    pthread_join(pid1,nullptr);
    pthread_join(pid2,nullptr);
    return 0;
}
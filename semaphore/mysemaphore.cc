#include "ring_buffer.hpp"
#include<cstdlib>
#include<unistd.h>
#include<time.h>

using namespace ns_ring_buffer;

void* Consumer(void* args)
{
    RingBuffer<int>* rb=(RingBuffer<int>*)args;
    while(true){
        int data=0;
        rb->Pop(&data);
        std::cout<<"消费数据："<<data<<std::endl;
        //sleep(1);
    }
}
void* Producer(void* args)
{
    RingBuffer<int>* rb=(RingBuffer<int>*)args;
    while(true){
        int data=rand()%20+1;
        std::cout<<"生产数据："<<data<<std::endl;
        rb->Push(data);
        sleep(1);
    }
}
int main()
{
    srand((long long)time(nullptr));
    //创建两个或者多个轻量级进程和一个环形数组
    pthread_t c,p;
    RingBuffer<int>* rb=new RingBuffer<int>;
    pthread_create(&c,nullptr,Consumer,(void*)rb);
    pthread_create(&p,nullptr,Producer,(void*)rb);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    return 0;
}
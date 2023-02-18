#pragma once
#include<iostream>
#include<queue>
#include<pthread.h>

#define CAPACITY 10

namespace ds_blockqueue{
    template<class T>
    class BlockQueue
    {
    private:
        std::queue<T> bq;//先入先出队列
        int cap;//容量
        //两个条件变量和一个互斥锁
        pthread_cond_t pc;
        pthread_cond_t cc;
        pthread_mutex_t mtx;
    public:
        BlockQueue()
            :cap(CAPACITY)
        {
            pthread_cond_init(&pc,nullptr);
            pthread_cond_init(&cc,nullptr);
            pthread_mutex_init(&mtx,nullptr);
        }
        ~BlockQueue()
        {
            pthread_cond_destroy(&pc);
            pthread_cond_destroy(&cc);
            pthread_mutex_destroy(&mtx);
        }
    private:
        void WaitProducer()
        {
            pthread_cond_wait(&pc,&mtx);
        }
        void WaitConsumer()
        {
            pthread_cond_wait(&cc,&mtx);
        }
        void WakeupProducer()
        {
            pthread_cond_signal(&pc);
        }
        void WakeupConsumer()
        {
            pthread_cond_signal(&cc);
        }
        void Lock()
        {
            pthread_mutex_lock(&mtx);
        }
        void Unlock()
        {
            pthread_mutex_unlock(&mtx);
        }
    private:
        bool IsFull()
        {
            return bq.size()==cap;
        }
        bool IsEmpty()
        {
            return bq.size()==0;
        }
    public:
        //当满时生产者需要等待消费者消费
        //当空时消费者需要等待生产者生产
        void Push(const T& in)
        {
            Lock();
            while(IsFull())//被挂起如果其他轻量级进程修改临界资源，使用if判断这个条件条件不一定是满足的使用while免得伪唤醒
            {
                WaitProducer();
            }
            bq.push(in);
            //生成一个就可以消费了
            if(bq.size()>=cap/2)
                WakeupConsumer(); 
            Unlock();
        }
        void Pop(T* out)
        {
            Lock();
            while(IsEmpty())////被挂起如果其他轻量级进程修改临界资源，使用if判断这个条件条件不一定是满足的使用while免得伪唤醒
            {
                WaitConsumer();
            }
            *out=bq.front();
            bq.pop();
            //消费一个就可以生产了
            if(bq.size()<=cap/2)
                WakeupProducer();
            Unlock();
        }
    };
}
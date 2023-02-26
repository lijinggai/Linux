#pragma once
#include<iostream>
#include<vector>
#include<pthread.h>
#include<semaphore.h>

#define RB_CAPACITY 10

namespace ns_ring_buffer{
    template<class T>
    class RingBuffer
    {
    private:
        std::vector<T> _v;
        sem_t blank;
        sem_t data; 
        pthread_mutex_t mtx;
        int _cap;
        //分别的下标记录位置
        int _p_index;
        int _c_index;
    public:
        RingBuffer(const T& cap=RB_CAPACITY)
            :_cap(cap)
            ,_v(cap)
        {
            _p_index=_c_index=0;
            //初始化
            sem_init(&blank,0,RB_CAPACITY);
            sem_init(&data,0,0);
            pthread_mutex_init(&mtx,nullptr);
        }
        ~RingBuffer()
        {
            sem_destroy(&blank);
            sem_destroy(&data);
            pthread_mutex_destroy(&mtx);
        }
        void Push(const T& in)
        {
            sem_wait(&blank);//--空位置
            //多线程保护临界资源
            pthread_mutex_lock(&mtx);
            _v[_p_index]=in;

            _p_index++;
            _p_index%=_cap;
            pthread_mutex_unlock(&mtx);
            sem_post(&data);//++数据
        }
        void Pop(T* out)
        {
            sem_wait(&data);//--数据
            //多线程保护临界资源
            pthread_mutex_lock(&mtx);
            *out=_v[_c_index];

            _c_index++;
            _c_index%=_cap;
            pthread_mutex_unlock(&mtx);
            sem_post(&blank);//++空位置

        }
    };
}
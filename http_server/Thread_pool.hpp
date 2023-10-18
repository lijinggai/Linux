#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>
#include "Log.hpp"
#include "Task.hpp"

#define NUM 5
class ThreadPool
{
private:
    void Lock()
    {
        pthread_mutex_lock(&_mt);
    }
    void Unlock()
    {
        pthread_mutex_unlock(&_mt);
    }
    void Wait()
    {
        pthread_cond_wait(&_cond, &_mt);
    }
    void Wakeup()
    {
        pthread_cond_signal(&_cond);
    }
    bool IfEmpty()
    {
        _q.empty();
    }
    void Pop(Task &t)
    {
        t = _q.front();
        _q.pop();
    }
    // 如果不是静态成员函数，那么参数实际有两个，一个是this指针
    static void *handler(void *arg)
    {
        Task t;
        pthread_detach(pthread_self());
        ThreadPool *tp = (ThreadPool *)arg;
        while (true)
        {           
            tp->Lock();
            while (tp->IfEmpty())
            {
                tp->Wait();
            }
            tp->Pop(t);
            tp->Unlock();
            //对队头数据进行处理
            t.ProcessOn();
        }
    }

public:
    void Push(int sock)
    {
        Task ts(sock);
        _q.push(ts);
        Wakeup();
    }
    void InitThread()
    {
        pthread_t pt[_num];
        for (int i = 0; i < _num; i++)
        {
            if (pthread_create(&pt[i], nullptr, handler, this) != 0)
            {
                LOG(ERROR, "pthread_create fail");
                exit(1);
            }
        }
    }
    static ThreadPool *GetInstance()
    {
        static pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
        if (_sigleton == nullptr)
        {
            pthread_mutex_lock(&_mutex);
            //双重判断，防止上面的判断成功线程被切换，调度回来时可能已经被其他线程new
            if(_sigleton == nullptr)
            {
                _sigleton = new ThreadPool;
                _sigleton->InitThread();
                LOG(INFO,"thread init ok");
            }
            pthread_mutex_lock;
        }
        return _sigleton;
    }

private:
    ThreadPool() : _num(NUM)
    {
        pthread_mutex_init(&_mt, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }
    ThreadPool(const ThreadPool &tp) = delete;
    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mt);
        pthread_cond_destroy(&_cond);
    }

private:
    int _num;
    std::queue<Task> _q;
    pthread_mutex_t _mt;
    pthread_cond_t _cond;
    static ThreadPool *_sigleton;
};
ThreadPool *ThreadPool::_sigleton = nullptr;
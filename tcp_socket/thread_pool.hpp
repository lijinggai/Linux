#pragma once
#include<iostream>
#include<queue>
#include<pthread.h>


namespace ns_thread_pool{
    const int Thread_Num=5;

    template<class T>
    class thread_pool{
    private:
        std::queue<T> _task;
        pthread_mutex_t _mtx;//互斥锁
        pthread_cond_t _cond;
    public:
        thread_pool()
        {
            pthread_cond_init(&_cond,nullptr);
            pthread_mutex_init(&_mtx,nullptr);
        }
        ~thread_pool()
        {
            pthread_cond_destroy(&_cond);
            pthread_mutex_destroy(&_mtx);
        }
    private:
        void Wakeup()
        {
            pthread_cond_signal(&_cond);
        }
        void Wait()
        {
            pthread_cond_wait(&_cond,&_mtx);
        }
        void Lock()//加锁
        {
            pthread_mutex_lock(&_mtx);
        }
        void Unlock()//解锁
        {
            pthread_mutex_unlock(&_mtx);
        }
    private:
        bool EmptyQueue(){
            return _task.empty();//判断任务队列是否为空
        }
        void Pop(T* out){
            *out=_task.front();//拿出队列头元素准备处理
            _task.pop();
        }

        //多线程的执行的函数参数只有一个，但是类会自带this指针
        //可以传入this指针，来访问非静态成员函数，但是不可以访问非静态成员
        static void* Run(void* agrs)
        {
            pthread_detach(pthread_self());//分离线程
            thread_pool<T>* tthis=(thread_pool<T> *)agrs;//this指针
            while(true){
                tthis->Lock();
                //条件变量使用while判断，以防伪唤醒
                while(tthis->EmptyQueue())
                {
                    tthis->Wait();
                }
                //拿出队列头元素准备处理
                T out;
                tthis->Pop(&out);
                tthis->Unlock();
                //数据是被一个在栈上的变量,以输出型函数拿出来的，是线程独有的不属于临界资源
                //处理数据
                out();
            }
        }
    public:
        void InitThreadPool(){
            //创建一批线程
            pthread_t tid[Thread_Num];
            for(int i=0;i<Thread_Num;i++)
            {
                pthread_create(tid+i,nullptr,Run,(void*)this);
            }
        }
        void Push(const T& in){
            //可以会有多个线程访问push
            Lock();
            _task.push(in);
            Unlock();
            Wakeup();
        }
    };
}
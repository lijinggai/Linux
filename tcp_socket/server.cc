#include<iostream>
#include<cerrno>
#include<unistd.h>
#include<signal.h>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include "Task.hpp"
using namespace ns_task;
#include "thread_pool.hpp"
using namespace ns_thread_pool;

#define PORT 8081
void serviceIO(int fd)
{
    //1.单进程version
    while(1)
    {
        char buffer[1024]={0};
        ssize_t s=read(fd,buffer,sizeof(buffer));
        if(s>0){
            buffer[s]=0;
            std::cout<<"client to server:"<<buffer<<std::endl;
            std::string message;
            message+="server to client:你好!";
            //给连接的客户端发一个你好
            write(fd,message.c_str(),message.length());
        }
        else if(s==0){//写端关闭读端读到文件结尾，再读返回0
            std::cout<<"client quit!"<<std::endl;
            break;
        }
        else{
            std::cerr<<"read "<<errno<<std::endl;
            break;
        }
    }
}
// void* HandlerRequest(void* agrs)
// {
//     pthread_detach(pthread_self());
//     int fd=*((int*)agrs);
//     serviceIO(fd);
//     close(fd);
// }
int main()
{
    //建立套接字
    int listen_sock=socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock<0)
    {
        std::cerr<<"socket"<<errno<<std::endl;
        return 1;
    }
    //bind服务器
    struct sockaddr_in local;

    local.sin_family=AF_INET;
    local.sin_port=htons(PORT);
    //local.sin_addr.s_addr=inet_addr(127.0.0.1);
    local.sin_addr.s_addr=INADDR_ANY;
    if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))<0)
    {
        std::cerr<<"bind"<<errno<<std::endl;
        return 2;
    }
    //listen状态
    listen(listen_sock,5);
    //signal(SIGCHLD,SIG_IGN);//忽略SIGCHLD信号，子进程将自动释放
    while(1)
    {
        struct sockaddr_in tmp;
        socklen_t tlen=sizeof(tmp);
        //建立连接

        int fd=accept(listen_sock,(struct sockaddr*)&tmp,&tlen);
        if(fd<0)
        {
            std::cerr<<"accept "<<errno<<std::endl;
            return 3;
        }
        std::cout<<"get a new link "<<std::endl;
        //线程池
        Task task(fd);
        thread_pool<Task> tp;
        tp.InitThreadPool();
        tp.Push(task);

        //3.多线程版本
        // pthread_t tid;
        // pthread_create(&tid,nullptr,HandlerRequest,(void*)&fd);
        //2.多进程version
        // pid_t pid = fork();
        // if(pid<0){
        //     continue;
        // }
        // else if(pid==0){//子进程
        //     close(listen_sock);
        //     //if(fork()>0)
        //     //    exit(0);//子进程创建就直接退出，创建的孙子进程执行后序代码，孙子进程变成孤儿进程被1号进程领养
        //     serviceIO(fd);
        //     close(fd);
        //     exit(0);
        // }
        // else{//父进程
        //     close(fd);//子进程的PCB以父进程的PCB做模板初始化，不是共享的
        //     //waitpid(&pid,NULL,0);//父进程等待子进程，子进程创建后再创建孙子进程执行后序代码，子进程秒退等待时间可以忽略不计
        // }
         
    }

    return 0;
}
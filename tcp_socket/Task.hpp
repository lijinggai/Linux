#include<iostream>
#include<string>
#include<sys/socket.h>
#include<sys/types.h>

namespace ns_task{
    class Task{
    private:
        int _sock;
    public:
        Task():_sock(-1)
        {}
        Task(int sock)   
            :_sock(sock) 
        {}
        void operator()()
        {
            //1.单进程version
            char buffer[1024]={0};
            ssize_t s=read(_sock,buffer,sizeof(buffer));
            if(s>0){
                buffer[s]=0;
                std::cout<<"client to server:"<<buffer<<std::endl;
                std::string message;
                message+="server to client:你好!";
                //给连接的客户端发一个你好
                write(_sock,message.c_str(),message.length());
            }
            else if(s==0){//写端关闭读端读到文件结尾，再读返回0
                std::cout<<"client quit!"<<std::endl;
            }
            else{
                std::cerr<<"read "<<errno<<std::endl;
            }
            close(_sock);
        }
    };
}
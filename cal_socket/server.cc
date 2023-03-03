#include "socket.hpp"
#include <unistd.h>
#include<pthread.h>

using namespace ns_socket;

void* Run(void* args)
{
    pthread_detach(pthread_self());
    int new_sock=*((int*)args);;
    while(1){
        Requist req;
        ssize_t s=read(new_sock,&req,sizeof(req));
        if(s==0)
        {
            std::cout<<"connect quit"<<std::endl;
            break;
        }
        else if(s!=sizeof(req))
        {
            std::cerr<<"server read"<<std::endl;
        }
        
        Response res;
        switch(req._op)
        {
            case '+':
                res._result=req._x+req._y;
                break;
            case '-':
                res._result=req._x+req._y;
                break;
            case '*':
                res._result=req._x+req._y;
                break;
            case '/':
                if(req._y==0)
                    res._code=1;
                else
                    res._result=req._x+req._y;
                break;
            default:
                break;
        }
        s=write(new_sock,&res,sizeof(res));
        if(s<0)
        {
            std::cerr<<"write server"<<std::endl;
            exit(2);
        }
    }
    close(new_sock);
}
int main(int argc,char* argv[])
{
    //创建套接字
    int sock=sock::Socket();
    //bind服务器
    sock::Bind(sock,argv[1]);
    //listen
    sock::Listen(sock);
    //accept
    while(1){
        int new_sock=sock::Accept(sock);
        std::cout<<"get a new link"<<std::endl;
        pthread_t tid;
        pthread_create(&tid,0,Run,(void*)&new_sock);
    }
    return 0;
}
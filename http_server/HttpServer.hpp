#include "TcpSocket.hpp"
#include "Protocol.hpp"
#include "Thread_pool.hpp"
#include<signal.h>
class HttpSocket
{
private:
    int _port;
    int _quit;

public:
    HttpSocket(int port) : _port(port), _quit(false)
    {
    }
    void InitServer()
    {
        //信号SIGPIPE需要进行忽略，如果不忽略，在写入时候，可能直接崩溃server
        signal(SIGPIPE, SIG_IGN); 
    }
    void loop()
    {   
        //创建、bind、监听套接字，获取listen套接字
        TcpSocket *segleton = TcpSocket::GetInstance(_port);
        int listen_socket = segleton->GetSocket();
        LOG(INFO,"build success,wait client");
        
        while (!_quit)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            //接受新套接字
            int new_socket = accept(listen_socket, (struct sockaddr *)&peer, &len);
            if(new_socket < 0)
                continue;
            ThreadPool::GetInstance()->Push(new_socket);
        }
    }
};
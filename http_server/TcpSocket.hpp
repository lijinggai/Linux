#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include"Log.hpp"

#define Backlog 5
class TcpSocket
{
private:
    void Socket()
    {
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket < 0)
        {
            LOG(FATAL,"socket error");
            exit(1);
        }
        // 快速重启
        int opt = 1;
        setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }
    void Bind()
    {
        struct sockaddr_in local;
        // 把套接字置为0；
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(_port);
        if (bind(_socket, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            LOG(FATAL,"bind error");
            exit(2);
        }
    }
    void Listen()
    {
        if (listen(_socket, Backlog) < 0)
        {
            LOG(FATAL,"listen error");
            exit(3);
        }
    }

public:
    void Init()
    {
        Socket();
        Bind();
        Listen();
    }
    static TcpSocket *GetInstance(int port)
    {
        static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        if (_sigleton == nullptr)
        {
            // 防止多进程访问临界资源
            pthread_mutex_lock(&lock);
            if (_sigleton == nullptr)
            {
                _sigleton = new TcpSocket(port);
                _sigleton->Init();
            }
            pthread_mutex_unlock(&lock);
        }
        return _sigleton;
    }
    int GetSocket()
    {
        return _socket;
    }
    ~TcpSocket()
    {
        if (_socket >= 0)
            close(_socket);
    }

private:
    TcpSocket(int port) : _socket(-1), _port(port)
    {
    }
    TcpSocket(const TcpSocket &s)
    {
    }

private:
    int _socket;
    int _port;
    // 单例模式指针
    static TcpSocket *_sigleton;
};
TcpSocket *TcpSocket::_sigleton = nullptr;
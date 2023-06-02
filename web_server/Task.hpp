#pragma once
#include <iostream>
#include "Protocol.hpp"
class Task
{
public:
    Task()
    {
    }
    Task(int sock) : _accept_sock(sock)
    {
    }
    ~Task()
    {
    }
    void ProcessOn()
    {
        _cb.ProcessOn(_accept_sock);
    }

private:
    int _accept_sock;
    Callback _cb;
};
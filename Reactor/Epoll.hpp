#pragma once
#include<iostream>
#include<sys/epoll.h>
#include<stdlib.h>
#include<unistd.h>
#include"Log.hpp"

class Epoll
{
    const static int gsize = 64;
    const static int gtimeout = 5000;

public:
    void CreateEpoll()
    {
        _epfd = epoll_create(gsize);
        if(_epfd < 0) exit(1);
        else LOG2(INFO, "create epoll success",_epfd);
    }
    bool AddEpoll(int fd, int evts)
    {
        epoll_event events;
        events.data.fd = fd;
        events.events = evts;
        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &events);
        if(n == 0)
            LOG2(INFO, "AddEpoll success", fd);
        return n == 0;
    }
    bool ModEpoll(int fd, int evts)
    {
        epoll_event events;
        events.data.fd = fd;
        events.events = evts;
        int n = epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &events);
        return n == 0;
    }
    bool DelEpoll(int fd)
    {
        int n = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
        return n == 0;
    }
    int WaitEpoll(epoll_event *etvs, int etvs_num)
    {
        int n = epoll_wait(_epfd, etvs, etvs_num, _timeout);
        return n;
    }

    Epoll(int timeout = gtimeout):_timeout(timeout)
    {}
    ~Epoll()
    {
        if(_epfd >= 0)
            close(_epfd);
    }
private:
    int _epfd;
    int _timeout;
};

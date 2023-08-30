#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<functional>
#include<errno.h>  
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>

#include"Sock.hpp"
#include"Epoll.hpp"
#include"Protocol.hpp"

//不活跃时间上限
#define INACTIVITY_TIME (5 * 60)

//前置声明
class TcpServer;
class Connection;

using func_t = std::function<void(Connection*)>;
using cals_t = std::function<void(std::string &, Connection*)>;

class Connection{
public:
    Connection(int fd = -1 ):_fd(fd), _ts(nullptr)
    {}
    void SetCallBack(func_t recver, func_t sender, func_t exception)
    {
        _recver = recver;
        _sender = sender;
        _exception = exception;
    }
    ~Connection()
    {
        if(_fd >= 0)
            close(_fd);
    }
public:
    int _fd;

    //读写异常回调方法
    func_t _recver;
    func_t _sender;
    func_t _exception;

    //接受缓冲区
    std::string _outbuff;
    //发送缓冲区
    std::string _inbuff;

    //TcpServer的回指指针，对写事件的关心是按需打开
    TcpServer *_ts;
    
    //连接最近活跃活动的时间
    time_t _times;
};

class TcpServer{
    const static int gport = 8080;
    const static int gnum = 128;
private:
    void Accepter(Connection * con)
    {
        while(true)
        {
            con->_times = time(nullptr);
            struct sockaddr_in tmp;
            socklen_t tlen = sizeof(tmp);
            int new_sock = accept(con->_fd, (struct sockaddr *)&tmp, &tlen);
            if(new_sock < 0)
            {
                //所以事件处理完毕
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                else if(errno == EINTR)//可能被信号中断，概率极小
                    continue;
                else
                {
                    std::cout << "accept fail , errno :" << errno << strerror(errno) << std::endl;
                    break;
                }
            } 
            else//添加到epoll模型和_connections中管理；
            {
                if(AddConnection(new_sock, std::bind(&TcpServer::Recver, this, std::placeholders::_1), 
                                std::bind(&TcpServer::Sender, this, std::placeholders::_1),
                                std::bind(&TcpServer::Exception, this, std::placeholders::_1)
                                ))
                    LOG2(INFO, "add connection success",new_sock);
                else
                    LOG2(RISK, "add connection fail", new_sock);
            }
        }
    }
    void Recver(Connection *con)
    {
        con->_times = time(nullptr);
        const int buff_size = 1024;
        char buff[buff_size];
        while(true)
        {
            ssize_t s = recv(con->_fd, buff, buff_size - 1, 0);
            if(s > 0)
            {
                buff[s] = 0;
                con->_outbuff += buff;
            }
            else if(s == 0)
            {
                LOG2(INFO, "写端关闭", con->_fd);
                con->_exception(con);
                return;
            }
            else
            {
                //读取完毕
                if(errno == EAGAIN || errno == EWOULDBLOCK )
                {
                    LOG2(INFO, "处理完毕", con->_fd);
                    break;
                }
                else if(errno == EINTR)
                    continue;
                else
                {
                    LOG2(ERROR, "recv fail ,fd: ", con->_fd);
                    con->_exception(con);
                    return;
                }
            }
        }
        std::cout << "fd: " << con->_fd << "outbuff: " << con->_outbuff <<std::endl;
        //对outbuff内的完整报文，进行处理
        std::vector<std::string> out;
        //分隔报文，函数在protocol.hpp
        SplitMessage(out, con->_outbuff);
        for(auto &s : out)
            _cb(s, con);//业务逻辑回调指针，在主函数

    }
    void Sender(Connection *con)
    {
        con->_times = time(nullptr);
        while(true)
        {
            ssize_t s = send(con->_fd, con->_inbuff.c_str(), con->_inbuff.size(), 0);
            if(s < 0)
            {
                //处理完毕
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                else if(errno = EINTR)
                    continue;
                else
                {
                    LOG2(ERROR,"send fail", con->_fd);
                    con->_exception(con);
                    return;
                }
            }
            else if( s == 0 ){
                LOG2(ERROR,"读端关闭连接", con->_fd);
                con->_exception(con);
                return;
            }else{
                //去除inbuff已发送内容
                con->_inbuff.erase(0, s);
                if(con->_inbuff.empty())
                    break;
            }
        }
        //处理完毕，去除对写事件的关心
        if(con->_inbuff.empty())
            con->_ts->EnableReadWrite(con->_fd, true, false);
        else
            con->_ts->EnableReadWrite(con->_fd, true, true);
    }
    void Exception(Connection *con)
    {
        int fd = con->_fd;
        //connections去除描述符
        auto pr = _connections.find(con->_fd);
        if(pr != _connections.end())
            _connections.erase(pr);

        //epoll关心去除
        _epoll.DelEpoll(con->_fd);

        //关闭fd,析构函数内释放
        //Connection释放
        delete con;
        LOG2(INFO, "资源全部释放", fd);
    }
public:
    TcpServer(int port = gport, int num = gnum):_port(gport), _evts_num(num)
    {
        //套接字，创建bind监听
        _listenSock = Sock::Socket();
        Sock::Bind(_listenSock,_port);
        Sock::Listen(_listenSock);

        //构建epoll模型
        _epoll.CreateEpoll();

        //listensock添加epoll模型和_connections管理起来
        AddConnection(_listenSock, std::bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr);

        //epoll_wait就绪队列,获取已就绪的事件
        _evts = new epoll_event[_evts_num];
    }
    ~TcpServer()
    {
        if(_listenSock >= 0)
            close(_listenSock);
        if(_evts != nullptr)
            delete[] _evts;
        for(auto &pr : _connections)
        {
            _connections.erase(pr.first);
            delete pr.second;
        }
    }

private:
    bool SetNonblock(int fd)
    {
        int fl = fcntl(fd, F_GETFL);
        if(fl < 0)
            return false;
        fcntl(fd, F_SETFL, fl|O_NONBLOCK);
        return true;
    }
    bool AddConnection(int fd, func_t recver, func_t sender, func_t exception)
    {
        //fd设为非阻塞
        if(!SetNonblock(fd))
        {
            close(fd);
            LOG2(RISK, "Set Connection fail", fd);
            return false;
        }

        //listensock构建connection对象，_connections管理起来
        Connection *con = new Connection(fd);
        con->SetCallBack(recver, sender, exception);
        //回指指针
        con->_ts = this;
        //时间
        con->_times = time(nullptr);
        _connections[fd] = con;

        //listensock添加epoll模型
        //任何多路转接的服务器，一般默认只会打开对读取事件的关心，写入事件会按需进行打开！
        if(!_epoll.AddEpoll(fd, EPOLLIN | EPOLLET))
        {
            _connections.erase(fd);
            close(fd);
            return false;
        }
        else return true;
    }
    bool IsConnectionExits(int fd)
    {
        return _connections.count(fd) == 1;
    }
    void LoopOnce()
    {
        int n = _epoll.WaitEpoll(_evts, _evts_num);
        //有事件就绪
        if(n > 0)
        {
            //LOG2(INFO, "epoll wait success",fd);
            for(int i=0; i<n; i++)
            {
                int fd = _evts[i].data.fd;
                int events = _evts->events;

                //连接关闭或者错误，改为读写统一处理，读写失败调异常处理；
                if( events & EPOLLHUP){
                    LOG2(INFO,"连接关闭",fd);
                    events |= (EPOLLIN | EPOLLOUT);
                }
                if( events & EPOLLERR){
                    LOG2(INFO,"错误",fd);
                    events |= (EPOLLIN | EPOLLOUT);
                } 

                if(_connections.count(fd) && events & EPOLLIN)
                {
                    if(IsConnectionExits(fd) && _connections[fd]->_recver != nullptr){
                        _connections[fd]->_recver(_connections[fd]);
                    }
                }
                if(_connections.count(fd) && events & EPOLLOUT)
                {
                    if(IsConnectionExits(fd) && _connections[fd]->_sender != nullptr)
                        _connections[fd]->_sender(_connections[fd]);
                }
            }
        }
        else if(n == 0)
        {
            LOG(INFO, "timeout");
        }
        else
        {
            LOG(FATAL,"epoll wait fail");
            exit(4);
        }
    }
public:
    //有响应（inbuff）开启对写的关系
    void EnableReadWrite(int fd, bool epollin, bool epollout)
    {
        uint32_t evts = (epollin ? EPOLLIN : 0) | (epollout ? EPOLLOUT : 0) | EPOLLET;
        _epoll.ModEpoll(fd, evts);
    }
    //派发器，入口
    void Dispatcher(cals_t cb)
    {
        _cb = cb;
        while(true)
        {
            //去除不活跃的连接
            DeleteInactivity();
            LoopOnce();
        }
    }
private:
    void DeleteInactivity()
    {
        //去除不活跃的连接
        std::unordered_map<int, Connection*> t = _connections;
        for(auto &pr : t)
        {
            if(pr.first == _listenSock)
                continue;
            //std::cout << pr.second->_times << " " << time(nullptr) <<std::endl;
            if( ( time(nullptr) - pr.second->_times ) > INACTIVITY_TIME)
            {
                LOG2(INFO, "不活跃连接", pr.second->_fd);
                pr.second->_exception(pr.second);
            }
        }
    }

private:
    int _listenSock;

    //epoll
    Epoll _epoll;
    //就绪队列
    epoll_event* _evts;
    int _evts_num;

    //管理connection对象
    std::unordered_map<int, Connection*> _connections;
    int _port;

    //业务处理的回调指针
    cals_t _cb;
};
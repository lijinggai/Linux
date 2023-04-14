#include "socket.hpp"
#include <unistd.h>
#include <sys/epoll.h>

using namespace ns_socket;
using namespace std;

#define SIZE 128
#define NUM 68

void Usage()
{
    cout << "usage: ./epoll_server port" << endl;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage();
        return -1;
    }
    int listen_sock = sock::Socket();
    sock::Bind(listen_sock, uint16_t(atoi(argv[1])));
    sock::Listen(listen_sock);

    // 建立epoll模型
    int epfd = epoll_create(SIZE);
    cout << "epfd: " << epfd << endl;
    // 设置fd对应的event的事件和fd
    struct epoll_event epevent;
    epevent.events = EPOLLIN | EPOLLOUT;
    epevent.data.fd = listen_sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &epevent) != 0)
    {
        cout << "listen_sock epoll_ctl fail" << endl;
        return -2;
    }

    volatile bool quit = false;
    struct epoll_event events[NUM];
    while (!quit)
    {
        int timeout = 1000;
        int wait_num = epoll_wait(epfd, events, NUM, timeout);
        if (wait_num > 0)
        {
            // cout<<"有事件就绪了"<<endl;
            for (int i = 0; i < wait_num; i++)
            {
                // 等待成功先拿出fd和判断是哪个事件成功
                int fd = events[i].data.fd;
                if (events[i].events & EPOLLIN)
                {

                    cout << fd << "号文件描述符读就绪" << endl;
                    // 新链接
                    if (fd == listen_sock)
                    {
                        cout << fd << "号文件描述符获取新链接" << endl;
                        int new_sock = sock::Accept(fd);
                        if (new_sock >= 0)
                        {
                            struct epoll_event add_event;
                            add_event.events = EPOLLIN;
                            add_event.data.fd = new_sock;

                            if (epoll_ctl(epfd, EPOLL_CTL_ADD, new_sock, &add_event) == 0)
                                cout << new_sock << "号链接被添加到epoll" << endl;
                            else
                            {
                                close(new_sock);
                                cout << "epoll_ctl fail,close" << new_sock << endl;
                            }
                        }
                    }
                    // 读取数据
                    else
                    {
                        cout << fd << "号文件描述符读取数据" << endl;
                        char buffer[1024] = {0};
                        ssize_t s = read(fd, buffer, sizeof(buffer) - 1);
                        if (s > 0)
                        {
                            buffer[s] = 0;
                            cout << fd << "client: " << buffer << endl;
                        }
                        else if (s == 0)
                        {
                            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                            close(fd);
                            cout << "对端关闭,已关闭文件描述符和在epoll中去除" << endl;
                        }
                        else
                        {
                            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                            close(fd);
                            cout << "读取失败,已关闭文件描述符和在epoll中去除" << endl;
                        }
                    }
                }
            }
        }
        else if (wait_num == 0)
            cout << "timeout ..." << endl;
        else
            cout << "epoll error" << endl;
    }
    close(epfd);
    close(listen_sock);

    return 0;
}
#include "socket.hpp"
#include <unistd.h>
#include <poll.h>

using namespace ns_socket;
using namespace std;
#define NUM 128

void Usage()
{
    std::cout << "Usage:"<< "./poll_server port" << std::endl;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage();
        return -2;
    }

    int sock_listen = sock::Socket();
    cout << "sock_liste: " << sock_listen << endl;
    sock::Bind(sock_listen, uint16_t(atoi(argv[1])));
    sock::Listen(sock_listen);

    struct pollfd poll_arr[NUM];
    for (int i = 0; i < NUM; i++)
    {
        poll_arr[i].fd = -1;
        poll_arr[i].events = 0;
        poll_arr[i].revents = 0;
    }

    poll_arr[0].fd = sock_listen;
    poll_arr[0].events = POLLIN;
    poll_arr[0].revents = 0;

    while (true)
    {
        int ret = poll(poll_arr, sizeof(poll_arr) / sizeof(poll_arr[0]), 2000);
        if (ret > 0)
        {
            for (int i = 0; i < NUM; i++) // 哪些文件描述符等待成功了并处理他
            {
                if (poll_arr[i].revents & POLLIN)
                {
                    if (poll_arr[i].fd == sock_listen) // 新链接
                    {
                        int new_sock = sock::Accept(sock_listen);
                        if (new_sock >= 0)
                        {
                            int tmp=0;
                            for (int tmp = 0; tmp < NUM; tmp++) // 新链接的文件描述符添加会fds数组
                            {
                                if (poll_arr[tmp].fd == -1)
                                {
                                    poll_arr[tmp].fd = new_sock;
                                    poll_arr[tmp].events = POLLIN;
                                    poll_arr[tmp].revents = 0;
                                    cout << "获得新链接: " << new_sock << endl;
                                    break;
                                }
                            }
                            if (tmp == NUM)
                            {
                                close(new_sock);
                                cout << "链接已满,请重试,关闭描述符: " << new_sock << endl;
                            }
                        }
                    }
                    else // 新数据读取
                    {
                        cout << poll_arr[i].fd << "号文件描述符等待成功" << endl;
                        char buffer[1024]{0};
                        ssize_t s = read(poll_arr[i].fd, buffer, sizeof(buffer) - 1);
                        if (s > 0)
                        {
                            buffer[s] = 0;
                            cout << buffer << endl;
                        }
                        else if (s == 0)
                        {
                            cout << "对端关闭,关闭文件描述符: " << poll_arr[i].fd << endl;
                            close(poll_arr[i].fd);
                            for (int j = 0; j < NUM; j++) // fds数组移除对应文件描述符
                            {
                                if (poll_arr[j].fd == poll_arr[i].fd)
                                {
                                    poll_arr[j].fd = -1;
                                    poll_arr[j].events = 0;
                                    poll_arr[j].revents = 0;
                                }
                            }
                        }
                        else
                        {
                            cout << "读取数据失败,关闭文件描述符: " << poll_arr[i].fd << endl;
                            close(i);
                            for (int j = 0; j < NUM; j++) // fds数组移除对应文件描述符
                            {
                                if (poll_arr[j].fd == poll_arr[i].fd)
                                {
                                    poll_arr[j].fd = -1;
                                    poll_arr[j].events = 0;
                                    poll_arr[j].revents = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (ret == 0)
        {
            cout << "没有等待成功文件描述符，继续" << endl;
            continue;
        }
        else
        {
            cout << "poll失败,终止进程" << endl;
            return -1;
        }
    }
    close(sock_listen);
    return 0;
}
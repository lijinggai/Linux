#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

void SetNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        cerr << "fcntl failed" << endl;
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}
int main()
{
    SetNonBlock(0);
    char buf[1024];
    while (1)
    {
        ssize_t s = read(0, buf, sizeof(buf) - 1);
        if (s > 0)
        {
            //去掉回车
            buf[s-1] = 0;
            printf("read success,echo: %s\n", buf);
        }
        else if (s == 0)
        {
            printf("write closed,errno:%d\n", errno);
        }
        else
        {
            //等待未就绪，这两个宏都被定义为11
            if(errno==EAGAIN||errno==EWOULDBLOCK)
            {
                cout<<"time out"<<endl;
            }
            else
            {
                printf("read failed,errno:%d\n", errno);
            }
        }
        sleep(1);
    }
    return 0;
    EAGAIN
}

// int main()
// {
//     char buffer[1024] = {0};
//     // 阻塞IO
//     while (1)
//     {
//         ssize_t s = read(0, buffer, sizeof(buffer) - 1);
//         if (s > 0)
//         {
//             //去掉回车
//             buffer[s-1] = 0;
//             printf("read success,echo:%s\n",buffer);
//         }
//         else if (s == 0)
//         {
//             printf("write closed,errno:%d\n", errno);
//         }
//         else
//         {
//             printf("read failed,errno:%d\n", errno);
//         }
//     }
//     return 0;
// }
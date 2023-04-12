#include "socket.hpp"
#include <unistd.h>
#include <sys/select.h>

using namespace ns_socket;
using namespace std;
#define NUM (sizeof(fd_set)*8)
void Usage()
{
    std::cout<<"Usage:"<<"./select_server port"<<std::endl;
}
int main(int argc,char* argv[])
{
    if(argc!=2){
        Usage();
        return -2;
    }
    //fd_set readfds;//读等待位图
    //std::cout<<sizeof(readfds)<<std::endl;//测试fd_set位图结构有多少个数

    int sock_listen=sock::Socket();
    cout<<"sock_liste: "<<sock_listen<<endl;
    sock::Bind(sock_listen,uint16_t(atoi(argv[1])));
    sock::Listen(sock_listen);

    fd_set readfds;//读等待位图
    int* fds_array=new int[NUM];//记录select的文件描述符
    for(int i=0;i<NUM;i++)
    {
        fds_array[i]=-1;
    }
    fds_array[0]=sock_listen;
    
    while(true)
    {
        FD_ZERO(&readfds);//情况位图结构
        int max_fd=fds_array[0];//最大文件描述符
        for(int i=0;i<NUM;i++)
        {
            if(fds_array[i]!=-1){
                max_fd=fds_array[i];
                FD_SET(fds_array[i],&readfds);
            }
        }

        timeval tm={1,0};
        int select_retrun=select(max_fd+1,&readfds,nullptr,nullptr,&tm);//第一个参数为最大文件描述符+1;
        if(select_retrun>0)
        {
            for(int sock=0;sock<NUM;sock++)//哪些文件描述符等待成功了并处理他
            {
                if(FD_ISSET(sock,&readfds))
                {
                    if(sock==sock_listen)//新链接
                    {
                        int new_sock=sock::Accept(sock_listen);
                        if(new_sock>=0)
                        {
                            int tmp=0;
                            for(int tmp=0;tmp<NUM;tmp++)//新链接的文件描述符添加会fds数组
                            {
                                if(fds_array[tmp]==-1)
                                {
                                    fds_array[tmp]=new_sock;
                                    cout<<"获得新链接: "<<new_sock<<endl;
                                    break;
                                }
                            }
                            if(tmp==NUM)
                            {
                                close(new_sock);
                                cout<<"链接已满,请重试,关闭描述符: "<<new_sock<<endl;
                            }
                        }
                    }
                    else//新数据读取
                    {
                        cout<<sock<<"号文件描述符等待成功"<<endl;
                        char buffer[1024]{0};
                        ssize_t s=read(sock,buffer,sizeof(buffer)-1);
                        if(s>0)
                        {
                            buffer[s]=0;
                            cout<<buffer<<endl;
                        }
                        else if(s==0)
                        {
                            cout<<"对端关闭,关闭文件描述符: "<<sock<<endl;
                            close(sock);
                            for(int i=0;i<NUM;i++)//fds数组移除对应文件描述符
                            {
                                if(fds_array[i]==sock)
                                    fds_array[i]=-1;
                            }
                        }
                        else
                        {
                            cout<<"读取数据失败,关闭文件描述符: "<<sock<<endl;
                            close(sock);
                            for(int i=0;i<NUM;i++)//fds数组移除对应文件描述符
                            {
                                if(fds_array[i]==sock)
                                    fds_array[i]=-1;
                            }
                        }
                    }
                }
            }
        }
        else if(select_retrun==0)
        {
            cout<<"没有等待成功文件描述符，继续"<<endl;
            continue;
        }
        else
        {
            cout<<"select失败,终止进程"<<endl;
            return -1;
        }
    }
    close(sock_listen);
    return 0;
}

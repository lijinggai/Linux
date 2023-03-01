#include<iostream>
#include<stdio.h>
#include<cerrno>
#include<unistd.h>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

void Usage()
{
    std::cout<<"usage:./client server_IP server_port"<<std::endl;
}
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        Usage();
        return 1;
    }
    //建立套接字
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        std::cerr<<"socket"<<errno<<std::endl;
        return 2;
    }
    //自动bind
    //连接服务器
    struct sockaddr_in local;
    local.sin_addr.s_addr=inet_addr(argv[1]);
    local.sin_port=htons(atoi(argv[2]));
    local.sin_family=AF_INET;
    connect(sock,(struct sockaddr*)&local,sizeof(local));


    //业务逻辑
    while(1)
    {
        char buffer[1024]={0};
        std::cout<<"请输入#";
        fgets(buffer,sizeof(buffer),stdin);
        write(sock,buffer,sizeof(buffer));

        char mes[1024]={0};
        read(sock,mes,sizeof(mes));
        std::cout<<mes<<std::endl;
    }
    return 0;
}
#include<iostream>
#include<cerrno>
#include<stdio.h>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

const uint16_t port=8080;
int main()
{
    int sock=socket(AF_INET,SOCK_DGRAM,0);//创建套接字
    if(sock<0)
    {
        std::cerr<<"socket fail: "<<errno<<std::endl;
        return 1;
    } 
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(port);
    //1.需要将人识别的点分十进制，字符串风格IP地址，转化为4字节整数IP
    //2.考虑大小端的问题
    //local.sin_addr.s_addr="82.156.168.132";
    //1.云服务器，不允许bind公网IP，另外，在一般编写也不会指明IP
    //2.一般主机有多个IP，如果只是bind一个IP，发给其他IP的数据就不会交给主机处理;
    //local.sin_addr.s_addr=inet_addr("82.156.168.132");
    //INADDR_ANY只要是发给这个主机的数据都会被处理
    local.sin_addr.s_addr=INADDR_ANY;
    //int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)//bind主机
    {
        std::cerr << "bind error : " << errno << std::endl;
        return 2;
    }

    //业务逻辑
    char message[1024];
    bool quit=false;
    while(!quit){
        struct sockaddr_in peer; 
        socklen_t len=sizeof(peer);
        //ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
        ssize_t s=recvfrom(sock,message,sizeof(message)-1,0,(struct sockaddr*)&peer,&len);
        std::string serevr_echo;
        if(s>0){
            message[s]=0;
            std::cout<<"client# "<<message<<std::endl;
            FILE* fd=popen(message,"r");
            char line[1024]={0};
            while(fgets(line,sizeof(line),fd)!=NULL)
            {
                serevr_echo+=line;
            }        
            pclose(fd);
        }else{
            std::cerr<<"recvfrom"<<errno<<std::endl;
            return 2;
        }

        //std::string tm="你好吗?";
        //std::cout<<"server to client: "<<tm<<std::endl;
        //ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
        sendto(sock,serevr_echo.c_str(),serevr_echo.size(),0,(struct sockaddr*)&peer,len);
    }
    return 0;
}
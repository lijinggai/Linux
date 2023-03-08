#include<iostream>
#include<cerrno>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

//client serverIP serverPort
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        std::cout<<"请按格式输入: client serverIP serverPort"<<std::endl;
        return 2;
    }
    int sock=socket(AF_INET,SOCK_DGRAM,0);//创建套接字
    if(sock<0)
    {
        std::cout<<"socket create errno: "<<errno<<std::endl;
        return 1;
    }
    //客户端不用显示bind，OS会自动bind；
    //服务器端会有规划，让port是没有被占用的，让别人来访问这个port；
    //客户端去访问服务器端，任意一个port就可以了
    //client正常发送数据的时候，OS会自动给你bind，采用随机端口的方式

        struct sockaddr_in server;
        server.sin_family=AF_INET;
        server.sin_port=htons(atoi(argv[2]));
        server.sin_addr.s_addr=inet_addr(argv[1]);
    while(1)
    {
        char message[1024];
        std::cout<<"请输入#";
        fgets(message,sizeof(message),stdin);

        sendto(sock,message,sizeof(message),0,(struct sockaddr*)&server,sizeof(server));

        struct sockaddr_in tmp;
        socklen_t tlen=sizeof(tmp);
        char buffer[2048];
        ssize_t s=recvfrom(sock,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&tmp,&tlen);
        if(s>0){
            std::cout<<buffer<<std::endl;
            buffer[s]=0;   
        }else{
            std::cerr<<"recvfrom"<<errno<<std::endl;
            //return 2;
        }
    }
    return 0;
}
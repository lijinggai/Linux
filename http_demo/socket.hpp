#pragma once
#include<iostream>
#include<cstdlib>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

namespace ns_socket{
    class sock{
    public:
        static int Socket()
        {
            int sock=socket(AF_INET,SOCK_STREAM,0);
            if(sock<0)
            {
                std::cerr<<"socket"<<std::endl;
                exit(1);
            }
            return sock;
        }
        static void Bind(int sock,int port)
        {
            struct sockaddr_in local;
            local.sin_family=AF_INET;
            local.sin_addr.s_addr=INADDR_ANY;
            local.sin_port=htons(port);
            if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
            {
                std::cerr<<"bind"<<std::endl;
                exit(2);
            }
        }
        static void Listen(int sock)
        {
            if(listen(sock,5)<0)
            {
                std::cerr<<"listen"<<std::endl;
                exit(3);
            }
        }
        static int Accept(int sock) 
        {
            struct sockaddr_in tmp;
            socklen_t tlen=sizeof(tmp);
            int new_sock=accept(sock,(struct sockaddr*)&tmp,&tlen);
            if(new_sock<0)
            {
                std::cerr<<"accept"<<std::endl;
                exit(4);
            }
            return new_sock;  
        }
        static void Connect(int sock,char* server_ip,char* server_port)
        {
            struct sockaddr_in local;
            local.sin_family=AF_INET;
            local.sin_addr.s_addr=inet_addr(server_ip);
            local.sin_port=htons(atoi(server_port));
            if(connect(sock,(struct sockaddr*)&local,sizeof(local))<0)
            {
                std::cerr<<"connect"<<std::endl;
                exit(5);
            }
            else
            {
                std::cout<<"connet success"<<std::endl;
            }
        }
    };
}
#include "socket.hpp"

#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

using namespace ns_socket;
using namespace std;

#define CAPACITY 1024 * 10
#define WWWROOT "./wwwroot/"
#define HOME_PAGE "index.html"

void *HandlerRequest(void *args)
{
    pthread_detach(pthread_self());
    int new_sock = *((int *)args);

    std::string response;
    response+="http/1.0 302 Permanently moved\n";
    response+="Location: https://www.baidu.com/\n";
    response+="\n";
    send(new_sock,response.c_str(),response.size(),0);
    // // 路径
    // std::string file_path = WWWROOT;
    // file_path += HOME_PAGE;
    // ifstream in(file_path.c_str());
    // if (in.is_open())
    // {
    //     // 获取文件属性
    //     struct stat page_stat;
    //     stat(file_path.c_str(), &page_stat);

    //     std::string http_response;
    //     http_response += "http/1.0 200 ok\n";
    //     http_response += "Content-Type: text/html\n";
    //     http_response += "Content-Longth: ";
    //     http_response += std::to_string(page_stat.st_size);
    //     http_response += "\n";
    //     http_response += "\n"; // 空行

    //     // http的有效载荷，正文
    //     std::string content;
    //     std::string line;
    //     while (std::getline(in, line))
    //     {
    //         content += line;
    //     }
    //     http_response += content;
    //     send(new_sock, http_response.c_str(), http_response.size(), 0);
    //     // char buffer[CAPACITY]={0};
    //     // ssize_t s=recv(new_sock,buffer,sizeof(buffer)-1,0);
    //     // if(s>0)
    //     // {
    //     //     buffer[s]=0;
    //     //     cout<<buffer<<endl;
    //     // }
    // }
    // else
    // {
    //     std::string http_response = "http/1.0 404 NOT FOUND\n";
    //     // 正文部分的数据类型
    //     http_response += "Content-Type: text/html; charset=utf8\n";
    //     http_response += "\n";
    //     http_response += "<html><p>你访问的资源不存在</p></html>";
    //     send(new_sock, http_response.c_str(), http_response.size(), 0);
    // }

    close(new_sock);
}
int main()
{
    int sock = sock::Socket();
    sock::Bind(sock, 8081);
    sock::Listen(sock);
    while (1)
    {
        int new_sock = sock::Accept(sock);
        cout << "get a new link" << endl;
        pthread_t tid;
        pthread_create(&tid, 0, HandlerRequest, (void *)&new_sock);
    }
    return 0;
}

#include "socket.hpp"
#include <unistd.h>

using namespace ns_socket;
using namespace std;
int main(int argc, char *argv[])
{
    int sock = sock::Socket();
    sock::Connect(sock, argv[1], argv[2]);
    while(1)
    {
        Requist req;
        cout<<"please enter data one"<<endl;
        cin>>req._x;
        cout<<"please enter operator"<<endl;
        cin>>req._op;
        cout<<"please enter data two"<<endl;
        cin>>req._y;

        ssize_t s=write(sock,&req,sizeof(req));
        if(s<0)
        {
            cerr<<"client write"<<endl;
            exit(1);
        }
        Response res{0,0};
        s=read(sock,&res,sizeof(res));
        cout<<"结果:"<<res._result<<"code"<<res._code<<endl;
    }
    return 0;
}
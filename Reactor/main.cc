#include<iostream>
#include"Tcpserver.hpp"


void calculator(Request &req, Response &res)
{
    switch (req._op)
    {
    case '+':
        res._result = req._x + req._y;
        break;
    case '-':
        res._result = req._x - req._y;
        break;
    case '*':
        res._result = req._x * req._y;
        break;
    case '/':
        if(req._y != 0)
            res._result = req._x / req._y;
        else 
            res._err = 1;
        break;
    default:
        break;
    }
}
//业务处理
void CalArguments(std::string &str, Connection *con)
{
    //请求报文反序列化
    Request req;
    //std::cout<<str <<std::endl;
    if(!req.Deserialize(str)){
        LOG2(ERROR, "deseroalize fail" ,con->_fd);
        return;
    }

    //对数据处理
    Response res;
    calculator(req, res);

    //响应报文序列化
    std::string s = res.Serialize();

    //添加到inbuff
    con->_inbuff += s;

    //一定有响应报文，打开写事件的关系
    con->_ts->EnableReadWrite(con->_fd, true, true);
}

int main()
{
    TcpServer *ts = new TcpServer;
    ts->Dispatcher(CalArguments);
    
    return 0;
}
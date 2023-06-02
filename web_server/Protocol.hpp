#pragma once
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Util.hpp"
#include "Log.hpp"

#define WEB_ROOT "webroot"
#define PAGE_404 "webroot/page_404.html"
#define PAGE_400 "webroot/page_400.html"
#define PAGE_500 "webroot/page_500.html"
#define HOME_PAGE "index.html"
#define OK 202
#define CLIENT_ERR 400
#define SERVER_ERR 500
#define NOT_FOUND 404
#define END_LINE "\n"

static std::string StatusCodeDesc(int status_code)
{
    std::string tmp;
    switch (status_code)
    {
    case 202:
        tmp = "OK";
        break;
    case 404:
        tmp = "NOT_FOUND";
        break;
    case 400:
        tmp = "CLIENT ERROR";
        break;
    case 500:
        tmp = "SERVER ERROR";
        break;
    default:
        break;
    }
    return tmp;
}
static std::string GetContentType(const std::string &type)
{
    std::unordered_map<std::string, std::string> ct{{".css", "text/css"}, {".html", "text/html"}, {".js", "application/x-javascript"}, {".jpg", "application/x-jpg"}};
    std::unordered_map<std::string, std::string>::iterator it = ct.find(type);
    if (it != ct.end())
        return it->second;
    return "text/html";
}

// 做为承装请求报文的容器
class HttpRequest
{
public:
    std::string _request_line;
    std::vector<std::string> _request_header;
    std::string _blank;
    std::string _body;

    // 把请求报头分离到键值对
    std::unordered_map<std::string, std::string> _um_header;
    // content-length
    int _content_length;

    // 请求分析
    std::string _method;
    std::string _uri;
    std::string _version;

    // uri的url部分
    std::string _url;
    // uri的参数
    std::string _uri_argments;
    // 客户端访问的url不一定是根目录，处理一下；
    std::string _request_path;

    std::string _cgi;
    // 需要返回文件的大小,有stat函数得到的
    int _size;

    HttpRequest() : _content_length(0), _size(0)
    {
    }
};
// 做为承装构建响应报文的容器

class HttpResponse
{
public:
    std::string _status_line;
    std::vector<std::string> _status_header;
    std::string _blank;
    std::string _body;

    // 状态码
    int _stat_code;
    // 是否需要进行cgi处理
    bool _cgi;
    HttpResponse() : _stat_code(0), _blank(END_LINE)
    {
    }
};

class EndPoint
{
private:
    // 读取请求行
    bool RecvRequestLine()
    {
        if (Util::RecvLine(_new_socket, _http_request._request_line))
        {
            // 去除\n
            _http_request._request_line.pop_back();
            LOG(INFO, _http_request._request_line);
            return true;
        }
        else
        {
            return false;
        }
    }

    // 读取请求报头
    bool RecvRequestHeader()
    {
        std::string tmp;
        while (tmp != "\n")
        {
            tmp.clear();
            if (Util::RecvLine(_new_socket, tmp))
            {
                if (tmp != "\n")
                {
                    // 去除\n
                    tmp.pop_back();
                    _http_request._request_header.push_back(tmp);
                    // LOG(INFO, tmp);
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    // 分析请求行
    void ParseRequestLine()
    {
        std::stringstream ss(_http_request._request_line);
        ss >> _http_request._method >> _http_request._uri >> _http_request._version;
        auto &method = _http_request._method;
        std::transform(method.begin(), method.end(), method.begin(), ::toupper); //::是全局作用域符

        // LOG(INFO, _http_request._method);
        // LOG(INFO, _http_request._uri);
        // LOG(INFO, _http_request._version);
    }
    // 分析请求报头
    void ParseRequestHeader()
    {
        for (int i = 0; i < _http_request._request_header.size(); i++)
        {
            std::string sub_out1;
            std::string sub_out2;
            if (Util::CutString(_http_request._request_header[i], ": ", sub_out1, sub_out2))
            {
                _http_request._um_header[sub_out1] = sub_out2;
            }
            else
                break;
        }
    }

    bool IsNeedRecvBody()
    {
        if (_http_request._method == "POST")
        {
            std::unordered_map<std::string, std::string>::iterator it = _http_request._um_header.find("Content-Length");
            _http_request._content_length = atoi(it->second.c_str());
            if (_http_request._content_length > 0)
            {
                return true;
            }
        }
        return false;
    }
    bool RecvRequestBody()
    {
        if (IsNeedRecvBody())
        {
            char ch;
            int num = _http_request._content_length;
            for (int i = 0; i < num; i++)
            {
                if (recv(_new_socket, &ch, 1, 0) > 0)
                {
                    _http_request._body.push_back(ch);
                    // LOG(INFO, _http_request._body);
                }
                else
                    return false;
            }
        }
        return true;
    }

private:
    void BuildStatusLine(int code)
    {
        // 状态行
        auto &status_line = _http_response._status_line;
        status_line += "HTTP/1.0";
        status_line += " ";
        status_line += std::to_string(code);
        status_line += " ";
        status_line += StatusCodeDesc(code);
        status_line += END_LINE;
    }
    void HandlerError(int code)
    {
        _http_response._cgi = false;
        if (code == NOT_FOUND)
            _http_request._request_path = PAGE_404;
        if (code == CLIENT_ERR)
            _http_request._request_path = PAGE_400;
        if (code == SERVER_ERR)
            _http_request._request_path = PAGE_500;
        // 响应报头
        auto &status_header = _http_response._status_header;
        struct stat st;
        int fd = open(_http_request._request_path.c_str(), O_RDONLY);
        if (fd >= 0)
        {
            if (stat(_http_request._request_path.c_str(), &st) == 0)
            {
                std::string line = "Content-Type: text/html";
                line += END_LINE;
                _http_response._status_header.push_back(line);

                _http_request._size = st.st_size;
                std::string content_length = "Content-Length: ";
                content_length += std::to_string(st.st_size);
                content_length += END_LINE;
                _http_response._status_header.push_back(content_length);
            }
        }
    }
    void HandlerOK()
    {

        // 响应报头
        auto &status_header = _http_response._status_header;
        std::string content_length = "Content-Length: ";
        if (_http_response._cgi)
        {
            content_length += std::to_string(_http_response._body.size());
        }
        else
        {
            content_length += std::to_string(_http_request._size);
        }
        content_length += END_LINE;
        status_header.push_back(content_length);

        std::string content_type = "Content-Type: ";
        // 读取后缀
        int pos = _http_request._request_path.rfind('.');
        if (pos != std::string::npos)
        {
            std::string suffix = _http_request._request_path.substr(pos);
            content_type += GetContentType(suffix);
        }
        else
            content_type += "text/html";
        content_type += END_LINE;
        status_header.push_back(content_type);
    }
    // 返回静态网页
    int ProcessNonCgi()
    {
        int fd = open(_http_request._request_path.c_str(), O_RDONLY);
        if (fd >= 0)
        {
            LOG(INFO, _http_request._request_path + " open success!");
            return OK;
        }
        return SERVER_ERR;
    }
    int ProcessCgi() //(重点)
    {
        int code = SERVER_ERR;
        auto &path = _http_request._request_path;
        auto &method = _http_request._method;
        auto &get_arguments = _http_request._uri_argments;
        auto &post_arguments = _http_request._body;
        auto &body = _http_response._body;

        // 把数据大小添加到环境变量内，返回被替换的子进程拿取
        std::string arguments_size;
        arguments_size += "ARGUMENTS_SIZE=";
        if (method == "GET")
            arguments_size += std::to_string(get_arguments.size());
        else if (method == "POST")
            arguments_size += std::to_string(post_arguments.size());
        putenv((char *)arguments_size.c_str());

        // 匿名管道是单信道通信,站在父进程的视角
        int output[2];
        int input[2];

        if (pipe(output) == -1)
        {
            LOG(ERROR, "pipe output[2] create fail");
            return SERVER_ERR;
        }
        if (pipe(input) == -1)
        {
            LOG(ERROR, "pipe input[2] create fail");
            return SERVER_ERR;
        }

        // 创建子进程后续用来进程替换
        pid_t pid = fork();
        if (pid == 0)
        {
            close(output[1]);
            close(input[0]);
            // 站在子进程的视角,进程替换数据被替换，提前使用dup2把:
            // output[0]->标准输入
            // input[1]->标准输出
            dup2(output[0], 0);
            dup2(input[1], 1);
            if (execl(path.c_str(), path.c_str(), nullptr) == -1)
            {
                std::cout << 2 << std::endl;
            }
            exit(0);
        }
        else if (pid > 0)
        {
            close(output[0]);
            close(input[1]);
            if (method == "GET")
            {
                write(output[1], get_arguments.c_str(), get_arguments.size());
            }
            if (method == "POST")
            {
                // post参数的大小可能很多，一次可能写不完
                int psize = post_arguments.size();
                int total = 0;
                ssize_t s = 0;
                while (total < psize && (s = write(output[1], post_arguments.c_str() + total, psize - total)) > 0)
                {
                    if (s <= 0)
                    {
                        LOG(ERROR, "write error");
                        break;
                    }
                    total += s;
                    // std::cout << psize << ":" << s << ":" << total << std::endl;
                }
            }
            // 读取cgi结果
            char ch;
            while (read(input[0], &ch, 1) > 0)
            {
                body.push_back(ch);
            }
            // std::cout<<body<<std::endl;
            int status;
            if (waitpid(pid, &status, 0) == pid)
            {
                if (WIFEXITED(status))
                {
                    if (WEXITSTATUS(status) == 0)
                        code = OK;
                    else
                        LOG(error, "error code discontinue");
                }
                else
                    LOG(error, "signal discontinue");
            }
            else
            {
                LOG(ERROR, "waitpid fail");
                return SERVER_ERR;
            }
            close(output[1]);
            close(input[0]);
        }
        else
        {
            // fork错误
            LOG(ERROR, "fork fail");
            return SERVER_ERR;
        }
        return code;
    }

public:
    // 读取
    void RecvRequest()
    {
        if (RecvRequestLine() && RecvRequestHeader())
        {
            ParseRequestLine();
            ParseRequestHeader();
            if (RecvRequestBody() == false)
            {
                _stop = true;
            }
        }
        else
            _stop = true;
    }

    // 构建
    void BuildResponse()
    {
        auto &method = _http_request._method;
        if (method != "GET" && method != "POST")
        {
            _http_response._stat_code = CLIENT_ERR;
            LOG(RISK, "method is not right");
            goto END;
        }
        if (method == "GET")
        {
            auto &uri = _http_request._uri;
            if (uri.find('?') != std::string::npos)
            {
                _http_response._cgi = true;
                Util::CutString(uri, "?", _http_request._url, _http_request._uri_argments);
            }
            else
                _http_request._url = uri;
        }
        else if (method == "POST")
        {
            _http_request._url = _http_request._uri;
            _http_response._cgi = true;
        }
        else
        {
            // do nothing
        }
        // 不一定是根目录，把url处理为符合服务器的url
        _http_request._request_path = WEB_ROOT;
        _http_request._request_path += _http_request._url;
        if (_http_request._request_path[_http_request._request_path.size() - 1] == '/')
        {
            // 把"/"或者"a/b/c/"这种目录都处理为，访问目录下的index.html
            _http_request._request_path += HOME_PAGE;
        }
        // 判断路径是否存在
        struct stat st;
        // 等于0代表执行函数成功，也等于有这个文件
        if (stat(_http_request._request_path.c_str(), &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                // 处理最后不是'/'的路径,例：/a/b/c
                _http_request._request_path += "/";
                _http_request._request_path += HOME_PAGE;
                stat(_http_request._request_path.c_str(), &st);
            }
            if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
            {
                // 特殊处理
                _http_request._cgi = true;
            }
            _http_request._size = st.st_size;
        }
        else
        {
            // 文件不存在
            LOG(RISK, _http_request._request_path);
            _http_response._stat_code = CLIENT_ERR;
            goto END;
        }
        // CGI处理
        if (_http_response._cgi)
        {
            _http_response._stat_code = ProcessCgi();
        }
        else
        {
            // 不需要CGI处理,返回静态网页
            _http_response._stat_code = ProcessNonCgi();
        }
    END:
        auto &code = _http_response._stat_code;
        // 构建状态行
        BuildStatusLine(code);
        // 构建响应报文
        switch (code)
        {
        case OK:
            HandlerOK();
            break;
        case NOT_FOUND:
            HandlerError(code);
            break;
        case CLIENT_ERR:
            HandlerError(code);
            break;
        case SERVER_ERR:
            HandlerError(code);
            break;
        default:
            break;
        }
        return;
    }
    void SendResponse()
    {
        if (send(_new_socket, _http_response._status_line.c_str(), _http_response._status_line.size(), 0) <= 0)
            return;
        std::cout << _http_response._status_line;
        for (auto &at : _http_response._status_header)
        {
            if (send(_new_socket, at.c_str(), at.size(), 0) <= 0)
                return;
            std::cout << at;
        }
        send(_new_socket, _http_response._blank.c_str(), _http_response._blank.size(), 0);
        std::cout << _http_response._blank;
        if (_http_response._cgi)
        {
            int size = _http_response._body.size();
            int total = 0;
            ssize_t s;
            std::cout << _http_response._body << std::endl;
            while (total < size && (s = send(_new_socket, _http_response._body.c_str() + total, size - total, 0)) > 0)
            {
                if (s <= 0)
                {
                    LOG(ERROR, "write error");
                    break;
                }
                total += s;
                // std::cout << size << ":" << s << ":" << total << std::endl;
            }
        }
        else
        {
            int fd = open(_http_request._request_path.c_str(), O_RDONLY);
            if (fd >= 0)
                sendfile(_new_socket, fd, 0, _http_request._size);
        }
    }
    bool GetStop()
    {
        return _stop;
    }

public:
    EndPoint(int sock) : _new_socket(sock), _stop(false)
    {
    }
    ~EndPoint()
    {
        if (_new_socket >= 0)
            close(_new_socket);
    }

private:
    HttpRequest _http_request;
    HttpResponse _http_response;

    int _new_socket;
    bool _stop;
};
class Callback
{
public:
    void ProcessOn(int new_socket)
    {
        EndPoint ep(new_socket);

        std::cout << "begin.........." << std::endl;
        ep.RecvRequest();
        if (ep.GetStop() == false)
        {
            ep.BuildResponse();
            ep.SendResponse();
        }

        std::cout << "end.........." << std::endl;

        // char buffer[4096];
        // ssize_t s=read(new_socket,buffer,sizeof(buffer)-1);
        // buffer[s]=0;
        // std::cout<<buffer<<std::endl;
    }
};
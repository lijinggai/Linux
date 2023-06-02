#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

class Util
{
public:
    static bool RecvLine(int sock, std::string &buffer)
    {
        char ch;
        while (ch != '\n')
        {
            ssize_t s = recv(sock, &ch, 1, 0);
            if (s > 0)
            {
                if (ch == '\r')
                {
                    // 窥探下一个元素但是不拿取;
                    recv(sock, &ch, 1, MSG_PEEK);
                    if (ch == '\n') // 处理\r\n->\n
                    {
                        recv(sock, &ch, 1, 0);
                    }
                    else // 处理\r->\n
                    {
                        ch = '\n';
                    }
                }
                buffer.push_back(ch);
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    static bool CutString(const std::string &source, const std::string sep, std::string &sub_out1, std::string &sub_out2)
    {
        int pos = source.find(sep);
        if (pos != std::string::npos)
        {
            sub_out1 = source.substr(0, pos);
            sub_out2 = source.substr(pos + sep.size());
            return true;
        }
        return false;
    }
};
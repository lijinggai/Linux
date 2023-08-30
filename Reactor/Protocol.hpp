#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<string.h>

//分隔符
#define SEG "x"
#define SEG_LEN strlen(SEG)

// #define SPACE " "
// #define SPACE_LEN strlen(SPACE)

#define RESULT "result: "
#define RESULT_LEN strlen(RESULT)
#define ERRCD "error_code: "
#define ERRCD_LEN strlen(ERRCD)


//分割报文
void SplitMessage(std::vector<std::string> &out, std::string &buff)
{
    while(true)
    {
        int pos = buff.find(SEG);
        if(pos == std::string::npos)
            break;
        out.push_back(buff.substr(0, pos));
        //需要删除分隔符
        buff.erase(0, pos+SEG_LEN);
    }
}



class Request
{
public:
    bool Deserialize(std::string &str)
    {
        std::string s1;
        std::string s2;
        bool flag = true;
        for(int i=0; i<str.size(); i++)
        {   
            //std::cout<< str[i] <<std::endl;
            if(str[i] == ' ' || str[i] == '\r' || str[i] == '\n')
                continue;
            else if(str[i] >= '0' && str[i] <= '9')
            {
                if(flag)
                    s1 += str[i];
                else
                    s2 += str[i];
            }
            else if(str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
            {
                _op = str[i];
                flag = false;
            }
            else
                return false;
        }
        if(s1.empty() || s2.empty())
            return false;
        _x = atoi(s1.c_str());
        _y = atoi(s2.c_str());
        return true;
        // int left = str.find(SPACE);
        // if(left == std::string::npos)
        //     return false;
        // _x = atoi(str.substr(0, left).c_str());
        // int right = str.rfind(SPACE);
        // if(right == std::string::npos)
        //     return false;
        // _y = atoi(str.substr(right+1).c_str());

        // if(left + SPACE_LEN >= str.size())
        //     return false;
        // else
        //     _op = str[left + SPACE_LEN];
        // return true;    
    }
    std::string Serialize(Request &req)
    {
        std::string s;
        s += std::to_string(_x);
        // s += SPACE;
        s += _op;
        // s += SPACE;
        s += std::to_string(_y);
        s += SEG;
        return s;
    }
public:
    Request()
    {}
    Request(int x, int y, char op):_x(x), _y(y), _op(op)
    {}
    ~Request()
    {}
public:
    int _x;
    int _y;
    char _op;
};

class Response
{
public:
    // 序列化
    std::string Serialize()
    {
        std::string s;
        s += RESULT;
        s += std::to_string(_result);
        s += ERRCD;
        s += std::to_string(_err);
        s += SEG;
        return s;
    }
    // 反序列化
    bool Deserialize(const std::string &s)
    {
        int pos = s.find(RESULT);
        if (pos == std::string::npos)
            return false;
        int pos2 = s.find(ERRCD);//错误码
        if (pos2 == std::string::npos)
            return false;

        _result = atoi(s.substr(pos + RESULT_LEN, pos2).c_str());
        _err = atoi(s.substr(pos2 + ERRCD_LEN).c_str());
        return true;
    }
public:
    Response(int result = 0, int err = 0 ) : _result(result), _err(err)
    {
    }
    ~Response() {}

public:
    double _result; // 计算结果
    int _err;
};

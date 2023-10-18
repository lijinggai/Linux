#include <iostream>
#include <unistd.h>
#include <string>
#include<cstdlib>
#include<cstdlib>

void GetArguments(std::string &arguments)
{
    char ch;
    int size = atoi(getenv("ARGUMENTS_SIZE"));
    for (int i = 0; i < size; i++)
    {
        read(0, &ch, 1);
        arguments.push_back(ch);
    }
}
void CutArguments(const std::string &in, const std::string &s, std::string& arg1, std::string& op, std::string& arg2)
{
    int pos = in.find(s);
    arg1 = in.substr(0, pos);
    int t = in.find(s, pos+1);
    op = in.substr(pos+1, t-pos-1);
    arg2 = in.substr(t + s.size());
}

void Dispose(std::string &arg1, std::string &op, std::string &arg2)
{
    std::string s1 = arg1.substr(arg1.find("=")+1);
    std::string s2 = arg2.substr(arg2.find("=")+1);
    int x = atoi(s1.c_str());
    int y = atoi(s2.c_str());

    char ch;
    std::string sop = op.substr(op.find("=")+1);
    if(sop == "%2B")
        ch = '+';
    else if(sop == "%2F")
        ch ='/';
    else if(sop == "%7C")
        ch = '|';
    else if(sop == "%5E")
        ch = '^';
    else
        ch= op[op.size()-1];

    //除零错误
    if(ch == '/' && y == 0 )
    {
        std::string t = "Division by zero erro";
        write(1, t.c_str(), t.size());
        return;
    }

    std::string ret;
    int res = 0;

    switch (ch)
    {
    case '+':
        res = x+y;
        break;
    case '-':
        res = x-y;
        break;
    case '*':
        res = x*y;
        break;
    case '/':
        res = x/y;
        break;
    case '|':
        res = x|y;
        break;
    case '^':
        res = x^y;
        break;   
    default:
        break;
    }
    ret += std::to_string(x);
    ret += ch;
    ret += std::to_string(y);
    ret += '=';
    ret += std::to_string(res);
    write(1, ret.c_str(), ret.size());
}

int main()
{
    std::string arguments;
    GetArguments(arguments);
    
    // 处理参数
    std::string arg1, arg2;
    std::string op;
    CutArguments(arguments, "&", arg1, op, arg2);

    Dispose(arg1, op, arg2);
    return 0;
}
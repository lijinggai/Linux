#include <iostream>
#include <unistd.h>
#include <string>
#include<cstdlib>
#include<cstdlib>

bool GetArguments(std::string &arguments)
{
    char ch;
    int size = atoi(getenv("ARGUMENTS_SIZE"));
    for (int i = 0; i < size; i++)
    {
        read(0, &ch, 1);
        arguments.push_back(ch);
    }
    if (arguments.size() == size)
        return true;
    else
        return false;
}
void CutArguments(const std::string &in, const std::string &op, std::string& arg1, std::string& arg2)
{
    int pos = in.find(op);
    arg1 = in.substr(0, pos);
    arg2 = in.substr(pos + op.size());
}
void CutArguments(const std::string &in, const std::string &op, std::string &name, int &value)
{
    int pos = in.find(op);
    name = in.substr(0, pos);
    value = atoi(in.substr(pos + op.size()).c_str());
}
bool PutCGIResult(int arg1, int arg2)
{
    std::string s;
    s+=std::to_string(arg1);
    s+="+";
    s+=std::to_string(arg2);
    s+="=";
    s+=std::to_string(arg1+arg2);
    if (write(1, s.c_str(), s.size()) < 0)
        return false;
    // if (write(1, arg2.c_str(), arg2.size()) < 0)
    //     return false;
    return true;
}
int main()
{
    std::cerr<<12<<std::endl;
    std::string arguments;
    if (GetArguments(arguments) == false)
        return 1;
    std::cerr<<arguments<<std::endl;
    // 处理参数
    std::string arg1, arg2;
    CutArguments(arguments, "&", arg1, arg2);

    std::string name1, name2;
    int value1, value2;
    CutArguments(arg1, "=", name1, value1);
    CutArguments(arg2, "=", name2, value2);

    // 返回结果
    if (PutCGIResult(value1, value2) == false)
        return 2;
    return 0;
}
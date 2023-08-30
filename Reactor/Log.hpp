#pragma once
#include <iostream>
#include <string>
#include <ctime>

#define INFO 1
#define RISK 2
#define ERROR 3
#define FATAL 4

#define LOG(level, message) Log(#level, message, __FILE__, __LINE__)
#define LOG2(level, message, fd) Log2(#level, message, fd, __FILE__, __LINE__)

void Log(std::string level, std::string massage, std::string file_name, int line_num)
{
    std::cout << "[日志等级:" << level << "][时间戳:" << time(nullptr) << "][描述信息:" << massage << "][对应文件:" << file_name << "][行数:" << line_num << "]" << std::endl;
}
void Log2(std::string level, std::string massage, int fd, std::string file_name, int line_num)
{
    std::cout << "[日志等级:" << level << "][时间戳:" << time(nullptr) << "][描述信息:" << massage << "][fd:"<< fd << "][对应文件:" << file_name << "][行数:" << line_num << "]" << std::endl;
}
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <functional>
#include <unordered_map>

typedef std::function<void()> func; // 函数对象
                                    // using func = std::function<void()>; // c++11新增写法

std::vector<func> callbacks;               // 若干个回调
std::unordered_map<int, std::string> desc; // 回调函数的注解
void readMySQL()
{
    std::cout << "sub process[" << getpid() << "]执行访问数据库的任务\n"
              << std::endl;
}

void executeUrl()
{
    std::cout << "sub process[" << getpid() << "]执行url解析\n"
              << std::endl;
}

void cal()
{
    std::cout << "sub process[" << getpid() << "]执行加密任务\n"
              << std::endl;
}

void save()
{
    std::cout << "sub process[" << getpid() << "]执行数据持久化任务\n"
              << std::endl;
}

void load() // 加载函数
{
    desc.insert({callbacks.size(), "readMySQL: 读取数据库"});
    callbacks.push_back(readMySQL);

    desc.insert({callbacks.size(), "executeUrl: 进行url解析"});
    callbacks.push_back(executeUrl);

    desc.insert({callbacks.size(), "cal: 进行加密计算"});
    callbacks.push_back(cal);

    desc.insert({callbacks.size(), "save: 进行数据的文件保存"});
    callbacks.push_back(save);
}

void showHandlers()
{
    for (const auto &iter : desc)
    {
        std::cout << iter.first << "\t" << iter.second << std::endl;
    }
}

int handlerSize()
{
    return callbacks.size();
}

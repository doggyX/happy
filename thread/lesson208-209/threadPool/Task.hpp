#pragma once

#include <iostream>
#include <functional>
#include <string>
#include "log.hpp"
typedef std::function<int(int, int)> funcc_t;
class Task
{

public:
    Task() {}
    Task(int x, int y, funcc_t func) : x_(x), y_(y), func_(func)
    {
        // std::cout << "制作任务完成 : " << x_ << " + " << y_ << " = ?" << std::endl;
    }
    void operator()(const std::string &name)
    {
        // std::cout << "线程 " << name << "处理完成, 结果是: " << x_ << "+" << y_ << " = " << func_(x_, y_) << std::endl;
        logMessage(WARNING, "%s处理完成 : %d + %d = %d | %s | %d", name.c_str(), x_, y_, func_(x_, y_), __FILE__, __LINE__);
    }

public:
    int x_;
    int y_;
    funcc_t func_;
};
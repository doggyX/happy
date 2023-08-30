#pragma once

#include <iostream>
#include <functional>
typedef std::function<int(int, int)> func_t;
class Task
{

public:
    Task() {}
    Task(int x, int y, func_t func) : x_(x), y_(y), func_(func)
    {
    }
    int operator()()
    {
        return func_(x_, y_);
    }

public:
    int x_;
    int y_;
    func_t func_;
};
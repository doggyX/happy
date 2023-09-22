#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <cstdio>

// typedef std::function<void *(void *)> func_t; // 系统提供的接口是c语言的,可能和这里的函数对象不兼容

typedef void *(*func_t)(void *); // 定义的是函数指针

class ThreadData
{
public:
    void *args_;
    std::string name_;
};
class Thread
{
public:
    // num是传编号 直接组成name_
    Thread(int num, func_t callback, void *args) : func_(callback)
    {
        char nameBuffer[64];
        snprintf(nameBuffer, sizeof nameBuffer, "Thread-%d", num);
        name_ = nameBuffer;

        tdata_.args_ = args;
        tdata_.name_ = name_;
    }

    void start()
    {
        pthread_create(&tid_, nullptr, func_, (void *)&tdata_);
    }

    void join()
    {
        pthread_join(tid_, nullptr);
    }

    std::string name()
    {
        return name_;
    }

    ~Thread()
    {
    }

private:
    std::string name_;
    pthread_t tid_;
    func_t func_;
    ThreadData tdata_;
};
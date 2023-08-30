#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include "Task.hpp"
#include "lockGuard.hpp"
// #include <mutex> // C++给做了封装的锁,这里不用

const int gDefaultCap = 5;

template <class T>
class BlockQueue
{
private:
    bool isQueueEmpty()
    {
        return bq_.size() == 0;
    }
    bool isQueueFull()
    {
        return bq_.size() == capacity_;
    }

public:
    BlockQueue(int capacity = gDefaultCap) : capacity_(capacity)
    {
        pthread_mutex_init(&mtx_, nullptr);
        pthread_cond_init(&isEmpty_, nullptr);
        pthread_cond_init(&isEmpty_, nullptr);
    }

    void push(const T &in)
    {
        // pthread_mutex_lock(&mtx_);
        // // 1. 先检测当前的临界资源是否能满足访问条件
        // while (isQueueFull())
        //     pthread_cond_wait(&isFull_, &mtx_);
        // // 为什么wait函数要带一把锁?当成功wait后,传入的锁,会被自动释放
        // // 被唤醒是在从哪里被阻塞挂起,从哪里唤醒,wait自动帮线程获取锁
        // // pthread_cond_wait 是一个函数,就可能调用失败
        // // pthread_cond_wait 可能存在伪唤醒的情况

        // // 2. 访问临界资源,要100%确定,资源是就绪的! 则上边的条件判断不能用if,要用while
        // bq_.push(in);
        // pthread_cond_signal(&isEmpty_);
        // pthread_mutex_unlock(&mtx_);
        {
            lockGuard lockguard(&mtx_); // 自动调用构造函数-加锁
            while (isQueueFull())
                pthread_cond_wait(&isFull_, &mtx_);
            bq_.push(in);
            pthread_cond_signal(&isEmpty_);
        } // 新知识 - 函数内大括号导致 提前析构 这又是一个局部

    } // 自动调用lockguard 析构函数-解锁

    void pop(T *out)
    {
        // pthread_mutex_lock(&mtx_);
        lockGuard lockguard(&mtx_);
        while (isQueueEmpty())
            pthread_cond_wait(&isEmpty_, &mtx_);
        *out = bq_.front();
        bq_.pop();
        pthread_cond_signal(&isFull_);
        // pthread_mutex_unlock(&mtx_);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&mtx_);
        pthread_cond_destroy(&isEmpty_);
        pthread_cond_destroy(&isFull_);
    }

private:
    std::queue<T> bq_;       // 阻塞队列
    int capacity_;           // 容量上限
    pthread_mutex_t mtx_;    // 通过互斥锁保证队列安全
    pthread_cond_t isEmpty_; // 用来表示bp是否为空
    pthread_cond_t isFull_;  // 用来表示bp是否满
};
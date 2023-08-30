#ifndef _Ring_QUEUE_HPP_
#define _Ring_QUEUE_HPP_

#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include "sem.hpp"

const int g_default_num = 5;

template <class T>
class RingQueue
{
public:
    RingQueue(int default_num = g_default_num) : ring_queue_(default_num), num_(default_num), c_step(0), p_step(0), space_sem_(default_num), data_sem_(0)
    {
        pthread_mutex_init(&c_lock, nullptr);
        pthread_mutex_init(&p_lock, nullptr);
    }
    ~RingQueue()
    {
        pthread_mutex_destroy(&c_lock);
        pthread_mutex_destroy(&p_lock);
    }
    // 生产者:空间资源
    void push(const T &in)
    {
        // static int index = 0;
        // ring_queue_[index++] = in;
        // index %= num_;
        space_sem_.p();              // p,v操作是原子性的 -- 认为先申请信号量  买票
        pthread_mutex_lock(&p_lock); // 加锁是是区域变得原子性  排队
        // 一定是竞争成功的生产者线程 -- 就一个
        ring_queue_[p_step++] = in;
        p_step %= num_;
        data_sem_.v();
        pthread_mutex_unlock(&p_lock);
    }

    // 消费者:数据资源
    void pop(T *out)
    {
        pthread_mutex_lock(&p_lock);
        // 一定是竞争成功的消费者线程 -- 就一个
        data_sem_.p();
        *out = ring_queue_[c_step++];
        c_step %= num_;
        space_sem_.v();
        pthread_mutex_unlock(&p_lock);
    }

    void debug()
    {
        std::cerr << "size : " << ring_queue_.size() << " num : " << num_ << std::endl;
    }

private:
    std::vector<T> ring_queue_;
    int num_;
    int c_step; // 消费下标
    int p_step; // 生产下标
    Sem space_sem_;
    Sem data_sem_;
    pthread_mutex_t c_lock;
    pthread_mutex_t p_lock;
};

#endif
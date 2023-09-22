#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unistd.h>
#include "thread.hpp"
#include "lockGuard.hpp"
#include "log.hpp"
// 本质是生产消费者模型
// run起来 push任务 再把任务派发给指定的线程

const int g_thread_num = 3;

template <class T>
class ThreadPool
{
public:
    pthread_mutex_t *getMutex()
    {
        return &lock;
    }
    bool isEmpty()
    {
        return task_queue_.empty();
    }
    void waitCond()
    {
        pthread_cond_wait(&cond, &lock);
    }
    T getTask()
    {
        T t = task_queue_.front();
        task_queue_.pop();
        return t;
    }

private:
    ThreadPool(int thread_num = g_thread_num) : num_(thread_num)
    {
        pthread_mutex_init(&lock, nullptr);
        pthread_cond_init(&cond, nullptr);
        for (int i = 1; i <= num_; i++)
        {
            threads_.push_back(new Thread(i, routine, this));
            // 为了能让static函数routine拿到任务队列, 需要在args传入this指针, 那么在构造函数是否能使用this指针呢?
            // 可以! 初始化列表, 即函数体前, 对象的空间都已经申请出来了, 后续工作都叫做赋值了, this指针已经指向对象!
        }
    }
    ThreadPool(const ThreadPool &other) = delete;
    const ThreadPool<T> &operator=(const ThreadPool &other) = delete;

public:
    static ThreadPool<T> *getThreadPool(int num = g_thread_num)
    {
        {
            lockGuard lockguard(&mutex); // 考虑一下多线程使用单例的过程 要加锁
            if (nullptr == thread_ptr)
            {
                thread_ptr = new ThreadPool<T>();
            }
        }
        return thread_ptr;
    }
    // 1. run()
    void run()
    {
        for (auto &iter : threads_)
        {
            iter->start();
            // std::cout << iter->name() << " 启动成功! " << std::endl;
            logMessage(NORMAL, "%s 启动成功", iter->name().c_str());
        }
    }

    // 消费过程
    // 这里有this指针 需要使用static修饰函数来去掉this指针
    static void *routine(void *args)
    {
        // 不要忘记(args)tdata中 有name 和 args
        ThreadData *td = (ThreadData *)args;
        ThreadPool *tp = (ThreadPool *)td->args_;
        while (true)
        {
            T task;
            // !!!
            // 我感觉这里有点小问题, 生产者和消费者用了同一把锁, 但是经过分析似乎并不会造成死锁, 建立在cond_signal 不会要求condwait的进程立即获得锁 ,
            // 但是我认为消费者生产者不应该用同一把锁, 会导致生产者和消费者是串行而不能并发
            // 发现这里我认为的生产者并不是真正的生产者 而是push这个行为 只是拷贝环节 不会占用太多时间
            {
                lockGuard lockguard(tp->getMutex());
                // cond
                while (tp->isEmpty())
                    tp->waitCond();

                // 读取任务
                task = tp->getTask(); // 任务队列是共享的 -> 将任务从共享, 拿到自己的私有空间
            }
            task(td->name_);
            // 认为每个task()应该提供仿函数去执行这个任务
            // 仿函数? 大概就是类的 operator() 函数
        }
    }
    // 线程池本身也是生产消费模型 外部发任务 线程竞争处理任务
    // 2. pushTask()
    void pushTask(const T &task)
    {
        lockGuard lockguard(&lock);
        task_queue_.push(task);
        pthread_cond_signal(&cond); // 有任务进来时去唤醒一个进程
    }

    // test function 临时join
    // void joins()
    // {
    //     for (auto &iter : threads_)
    //     {
    //         iter->join();
    //     }
    // }

    ~ThreadPool()
    {
        for (auto &iter : threads_)
        {
            iter->join();
            delete iter;
        }
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond);
    }

private:
    std::vector<Thread *> threads_;
    int num_;
    std::queue<T> task_queue_;

    static ThreadPool<T> *thread_ptr; // 单例模式
    static pthread_mutex_t mutex;

    pthread_mutex_t lock;
    pthread_cond_t cond; // 这里通过条件变量 使得来任务时通知休眠的进程去处理任务
};

template <typename T>
ThreadPool<T> *ThreadPool<T>::thread_ptr = nullptr;

template <typename T>
pthread_mutex_t ThreadPool<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

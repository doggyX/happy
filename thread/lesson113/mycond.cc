/*
2023/8/16
2023/8/17
lesson 35(1-13) 同步 生产者消费者模型
条件变量 生产者消费者模型
其实cond_wait应在加锁和解锁之间
*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <thread>
#include <time.h>
#include <string>
#include <assert.h>

using namespace std;

#define TNUM 4
typedef void (*func_t)(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond);
volatile bool quit = false;

class ThreadData
{
public:
    ThreadData(const std::string &name, func_t func, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
        : name_(name), func_(func), pmtx_(pmtx), pcond_(pcond)
    {
    }

public:
    std::string name_;
    func_t func_;
    pthread_mutex_t *pmtx_;
    pthread_cond_t *pcond_;
};

// void func1(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
// {
//     while (!quit)
//     {
//         pthread_cond_wait(pcond, pmtx); // 默认该线程在执行时, wait代码被执行, 当前线程会被立即阻塞--具体是被放进某个队列里,具有顺序性
//         std::cout << name << " running -- 播放" << std::endl;
//         // sleep(1);
//     }
// }

// 其实cond_wait应在加锁和解锁之间

void func1(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        // v2:
        pthread_mutex_lock(pmtx);
        // if(临界资源是否就绪 -- )
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行时, wait代码被执行, 当前线程会被立即阻塞--具体是被放进某个队列里,具有顺序性
        std::cout << name << " running -- 播放" << std::endl;
        pthread_mutex_unlock(pmtx);
        // sleep(1);
    }
}

void func2(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行时, wait代码被执行, 当前线程会被立即阻塞
        std::cout << name << " running -- 下载" << std::endl;
        pthread_mutex_unlock(pmtx);
        // sleep(1);
    }
}

void func3(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行时, wait代码被执行, 当前线程会被立即阻塞
        std::cout << name << " running -- 显示" << std::endl;
        pthread_mutex_unlock(pmtx);
        // sleep(1);
    }
}

void func4(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行时, wait代码被执行, 当前线程会被立即阻塞
        std::cout << name << " running -- 扫描用户信息" << std::endl;
        pthread_mutex_unlock(pmtx);
        // sleep(1);
    }
}

void *Entry(void *args)
{
    ThreadData *td = (ThreadData *)args; // td在每一个线程自己私有的栈空间中保存
    td->func_(td->name_, td->pmtx_, td->pcond_);
    delete td; // 这是一个函数, 最后要返回
    return nullptr;
}

int main()
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    pthread_mutex_init(&mtx, nullptr); // 局部的定于
    pthread_cond_init(&cond, nullptr);
    pthread_t tids[TNUM];
    func_t funcs[TNUM] = {func1, func2, func3, func4};
    for (int i = 0; i < TNUM; i++)
    {
        std::string name = "Thread ";
        name += std::to_string(i + 1);
        ThreadData *td = new ThreadData(name, funcs[i], &mtx, &cond);
        pthread_create(tids + i, nullptr, Entry, (void *)td);
    }

    // ctrl new thread

    sleep(3); // 这5秒没有线程做事情
    int cnt = 10;
    while (cnt)
    {
        pthread_cond_broadcast(&cond); // 唤醒所有在条件变量等待的进程
        // pthread_cond_signal(&cond);                                        // 唤醒一个进程 只需要在意条件变量 调度是cond在队列里调度的
        std::cout << "resume thread run code .... " << cnt-- << std::endl; // 继续
        sleep(1);
    }
    quit = true;
    pthread_cond_broadcast(&cond); // 唤醒所有在条件变量等待的进程

    std::cout << "ctrl done" << std::endl;
    for (int i = 0; i < TNUM; i++)
    {
        pthread_join(tids[i], nullptr);
        std::cout << "thread " << i + 1 << " quit!" << endl;
    }

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);
    return 0;
}

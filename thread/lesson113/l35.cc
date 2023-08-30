#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>

#define TNUM 4
typedef void (*func_t)(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond);
volatile bool quit = false;

// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

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

void func1(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        // wait一定要在加锁和解锁之间进行wait！
        // v2:
        pthread_mutex_lock(pmtx);
        // if(临界资源是否就绪-- 否) pthread_cond_wait
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        std::cout << name << " running -- 播放" << std::endl;
        pthread_mutex_unlock(pmtx);
    }
}

void func2(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        if (!quit)
            std::cout << name << " running  -- 下载" << std::endl;
        pthread_mutex_unlock(pmtx);
    }
}
void func3(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        std::cout << name << " running  -- 刷新" << std::endl;
        pthread_mutex_unlock(pmtx);
    }
}
void func4(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); // 默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        std::cout << name << " running  -- 扫码用户信息" << std::endl;
        pthread_mutex_unlock(pmtx);
    }
}

void *Entry(void *args)
{
    ThreadData *td = (ThreadData *)args;         // td在每一个线程自己私有的栈空间中保存
    td->func_(td->name_, td->pmtx_, td->pcond_); // 它是一个函数，调用完成就要返回！
    delete td;
    return nullptr;
}

int main()
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    pthread_mutex_init(&mtx, nullptr);
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

    sleep(5);

    // ctrl new thread
    int cnt = 10;
    while (cnt)
    {
        std::cout << "resume thread run code ...." << cnt-- << std::endl;
        pthread_cond_signal(&cond);
        // pthread_cond_broadcast(&cond);
        sleep(1);
    }

    std::cout << "ctrl done" << std::endl;
    quit = true;
    pthread_cond_broadcast(&cond);

    for (int i = 0; i < TNUM; i++)
    {
        pthread_join(tids[i], nullptr);
        std::cout << "thread: " << tids[i] << "quit" << std::endl;
    }

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);

    return 0;
}

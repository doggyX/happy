/*
2023/8/13
2023/8/15
2023/8/16
lesson 34(1-12) 互斥
定义锁 pthread_mutex_init
加锁 pthread_mutex_lock
加锁保护:加锁的时候,一定要保证加锁的粒度,越小越好!
*/

// 加锁就是串行执行了吗？加锁了之后，线程在临界区中，是否会切换，会有问题吗？原子性的体现
// 加锁了之后，线程在临界区中，是否会切换，会有问题吗?
// 会切换，不会！第一次理解：虽然被切换了，但是你是持有锁被切换的, 所以其他抢票线程要执行临界区代码，
// 也必须先申请锁，锁它是无法申请成功的，所以，也不会让其他线程进入临界区，就保证了临界区中数据一致性！

// 我是一个线程，我不申请锁，就是单纯的访问临界资源！-- 错误的编码方式

// 在没有持有锁的线程看来，对我最有意义的情况只有两种：1. 线程1没有持有锁(什么都没做) 2. 线程1释放锁(做完)，此时我可以申请锁！
// (这两个情况有原子性的体现)

// 加锁就是串行执行了吗？ 是的，执行临界区代码一定是串行的！

// 要访问临界资源，每一个线程都必须现申请锁，每一个线程都必须先看到同一把锁&&访问它，锁本身是不是就是一种共享资源？
// 谁来保证锁的安全呢？？所以，为了保证锁的安全，申请和释放锁，必须是 原子的！！！自己保证
// 如何保证？？锁是如何实现的？内存和寄存器的交换 是 exchange(一条汇编，导致1只有一份，1被拿走了，保存在上下文，被切换也没有关系)
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <thread> // c++的线程库 -- 封装 -- 底层也是使用的原生线程库(pthread) 编译要加上 -lpthread
#include <time.h>
#include <string>
#include <assert.h>

using namespace std;

class ThreadData
{
public:
    ThreadData(const std::string &n, pthread_mutex_t *pm) : tname(n), pmtx(pm)
    {
    }

public:
    std::string tname; // 这里是私有的话,实验得 不可解锁!
    pthread_mutex_t *pmtx;
};

#define THREAD_NUM 5
// 锁的全局定义 (有局部) static也可以这样定义
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // 定义一把锁 pthread_mutex_t是原生线程库提供的一个数据类型
int tickets = 10000;                             // 并发访问,会导致数据不一致的问题! 临界资源

void *getTickets(void *args)
{
    ThreadData *ptd = (ThreadData *)args;
    // 重入了
    while (true) // 是临界区
    {
        // 对下面的代码加锁 代码只能串行执行
        int n = pthread_mutex_lock(ptd->pmtx);
        assert(n == 0);
        if (tickets > 0) // 1. 判断的本质也是计算的一种
        {
            usleep(rand() % 1500); // 微秒 1毫秒
            printf("%s,  %d\n", ptd->tname.c_str(), tickets);
            tickets--; // 2. 也可能出现问题
            n == pthread_mutex_unlock(ptd->pmtx);
            assert(n == 0);
        }
        else
        {
            n == pthread_mutex_unlock(ptd->pmtx);
            assert(n == 0);
            break;
        }

        // 抢完票,其实还需要后续的动作
        usleep(rand() % 2000);

        // pthread_mutex_unlock(&mtx); // 不能放在这里 若上述判断else break了,就无法解锁了会影响其他线程
    }
    delete ptd;
    return nullptr;
}

int main()
{
    time_t start = time(nullptr);
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, nullptr); // 必须这样 , 还需要destroy
    srand((unsigned long)time(nullptr) ^ getpid() ^ 0x147);
    pthread_t t[THREAD_NUM];
    // 多线程抢票的逻辑

    for (int i = 0; i < THREAD_NUM; i++)
    {
        std::string name = "thread ";
        name += std::to_string(i + 1);
        ThreadData *ptd = new ThreadData(name, &mtx);
        pthread_create(t + i, nullptr, getTickets, (void *)ptd);
    }
    // pthread_create(&t2, nullptr, getTickets, (void *)"thread two");
    // pthread_create(&t3, nullptr, getTickets, (void *)"thread three");

    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(t[i], nullptr);
    }
    // pthread_join(t1, nullptr);
    // pthread_join(t2, nullptr);
    // pthread_join(t3, nullptr);
    pthread_mutex_destroy(&mtx);

    time_t end = time(nullptr);
    cout << "cast: " << (int)(end - start) << endl; // 是秒级别
}

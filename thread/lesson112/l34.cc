#include <iostream>
#include <thread>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <cassert>
#include <cstdio>

using namespace std;
// 如果多线程访问同一个全局变量，并对它进行数据计算，多线程会互相影响吗？
// 加锁保护：加锁的时候，一定要保证加锁的粒度，越小越好！！
// pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // pthread_mutex_t 就是原生线程库提供的一个数据类型

// 加锁就是串行执行了吗？加锁了之后，线程在临界区中，是否会切换，会有问题吗？原子性的体现
// 加锁了之后，线程在临界区中，是否会切换，会有问题吗?
// 会切换，不会！第一次理解：虽然被切换了，但是你是持有锁被切换的, 所以其他抢票线程要执行临界区代码，
// 也必须先申请锁，锁它是无法申请成功的，所以，也不会让其他线程进入临界区，就保证了临界区中数据一致性！
// 我是一个线程，我不申请锁，就是单纯的访问临界资源！-- 错误的编码方式
// 在没有持有锁的线程看来，对我最有意义的情况只有两种：1. 线程1没有持有锁(什么都没做) 2. 线程1释放锁(做完)，此时我可以申请锁！
// 加锁就是串行执行了吗？ 是的，执行临界区代码一定是串行的！
// 要访问临界资源，每一个线程都必须现申请锁，每一个线程都必须先看到同一把锁&&访问它，锁本身是不是就是一种共享资源？
// 谁来保证锁的安全呢？？所以，为了保证锁的安全，申请和释放锁，必须是 原子的！！！自己保证
// 如何保证？？锁是如何实现的？
int tickets = 10000; // 在并发访问的时候，导致了我们数据不一致的问题！临界资源

#define THREAD_NUM 800

class ThreadData
{
public:
    ThreadData(const std::string &n, pthread_mutex_t *pm) : tname(n), pmtx(pm)
    {
    }

public:
    std::string tname;
    pthread_mutex_t *pmtx;
};

void *getTickets(void *args)
{
    // int myerrno = errno;
    ThreadData *td = (ThreadData *)args;
    while (true)
    {
        // 抢票逻辑
        int n = pthread_mutex_lock(td->pmtx);
        assert(n == 0);
        // 临界区
        if (tickets > 0) // 1. 判断的本质也是计算的一种
        {
            usleep(rand() % 1500);
            printf("%s: %d\n", td->tname.c_str(), tickets);
            tickets--; // 2. 也可能出现问题
            n = pthread_mutex_unlock(td->pmtx);
            assert(n == 0);
        }
        else
        {
            n = pthread_mutex_unlock(td->pmtx);
            assert(n == 0);
            break;
        }

        // 抢完票，其实还需要后续的动作
        usleep(rand() % 2000);
        //  errno = myerrno;
    }
    delete td;
    return nullptr;
}

int main()
{
    time_t start = time(nullptr);
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, nullptr);

    srand((unsigned long)time(nullptr) ^ getpid() ^ 0x147);
    pthread_t t[THREAD_NUM];
    // 多线程抢票的逻辑
    for (int i = 0; i < THREAD_NUM; i++)
    {
        std::string name = "thread ";
        name += std::to_string(i + 1);
        ThreadData *td = new ThreadData(name, &mtx);
        pthread_create(t + i, nullptr, getTickets, (void *)td);
    }

    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(t[i], nullptr);
    }

    pthread_mutex_destroy(&mtx);

    time_t end = time(nullptr);

    cout << "cast: " << (int)(end - start) << "S" << endl;
}

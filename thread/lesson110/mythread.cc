/*
2023/8/8
lesson 33(1-10) 线程控制
1. 线程谁先运行与调度器相关
2. 线程一旦异常,可能导致整个进程退出
3. 线程在创建并执行的时候,线程也是需要进行等待的,如果主线程不等待,即会引起类似于僵尸进程的问题,导致内存泄漏
pthread_join
线程 vs 进程 线程传参和return的类型十分多变 线程等待不需要知道新线程的退出是否异常，因为他崩我也崩
不要调用exit,exit是终止进程的！-- pthread_exit
pthread_cancel 主动删
全局数据所有线程共享
__thread修饰全局遍历,使每个线程都有一份 -- 线程的局部存储
execl()呢?进程完全被替换
主线程不想join线程了,pthread_detch(pthread_self()) -- 新线程内调用 主线程不用等他了
互斥与同步开头
*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <thread> // c++的线程库 -- 封装 -- 底层也是使用的原生线程库(pthread) 编译要加上 -lpthread

using namespace std;

int tickets = 10000; // 并发访问,会导致数据不一致的问题!

void *getTickets(void *args)
{
    (void)args;
    // 重入了
    while (true)
    {
        if (tickets > 0)
        {
            usleep(1000); // 微秒 1毫秒
            printf("tid: %p, tickets : %d\n", pthread_self(), tickets);
            tickets--;
        }
        else
            break;
    }
}

int main()
{
    pthread_t t1, t2, t3;
    // 多线程抢票的逻辑
    pthread_create(&t1, nullptr, getTickets, nullptr);
    pthread_create(&t2, nullptr, getTickets, nullptr);
    pthread_create(&t3, nullptr, getTickets, nullptr);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
}

// // c++ 调用

// void fun()
// {
//     while (true)
//     {
//         cout << "hello new thread" << endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     std::thread t(fun);

//     t.join();
// }

// pthread库调用
// __thread int g_val = 0;
// // __thread修饰全局遍历,使每个线程都有一份

// void *threadRoutine(void *args)
// {
//     pthread_detach(pthread_self());

//     int i = 0;
//     i /= 0; // 分离了还是会因为异常导致进程退出
//     // int *data = new int[10];
//     while (true)
//     {
//         cout << "新线程: " << (char *)args << " running ... tid:  " << pthread_self() << endl;
//         sleep(1);
//         break;
//         // int a = 100;
//         // a /= 0;
//         // data[i] = i;
//         // if (i++ == 9)
//         //     break;
//     }
//     // exit(10);
//     // 不要调用exit,exit是终止进程的！
//     pthread_exit((void *)13);
//     cout << "new thread quit ... " << endl;
//     // return (void *)data; // 谁等我我返回给谁,一般是main thread, main thread如何获取呢?pthread_join
//     return nullptr;
// }

// int main()
// {
//     // 我们不是用的Linux自带的创建线程的接口，是pthread库的接口
//     pthread_t tid; // 本质是一个地址！
//     pthread_create(&tid, nullptr, threadRoutine, (void *)"thread 1");
//     // printf("%lu, %p\n", tid, tid);

//     int count = 0;
//     while (true)
//     {
//         cout << "main线程: "
//              << " running ... main tid: " << pthread_self() << endl;
//         sleep(1);
//         count++;
//         if (count >= 100)
//             break;
//     }
//     int n = pthread_join(tid, nullptr);
//     cout << " n : " << n << "errstring: " << strerror(n) << endl;
//     // pthread_cancel(tid);
//     // cout << "pthread cancle " << tid << endl;
//     // // 线程被取消，join的时候，退出码为-1 -- PTHREAD_CANCELED ((void *)-1)
//     // // 默认会阻塞等待新线程退出
//     // void *ret = nullptr;
//     // pthread_join(tid, (void **)&ret); // (第二个参数是void **,输出型参数! -- retval)
//     // cout << "main thread wait done ... main quit ... : new thread quit : " << (long long)ret << endl;
//     // sleep(5);

//     // for (int i = 0; i < 10; i++)
//     // {
//     //     cout << ret[i] << endl;
//     // }
//     // 1. 线程谁先运行与调度器相关
//     // 2. 线程一旦异常, 可能导致整个进程退出
//     // 3. 线程在创建并执行的时候,线程也是需要进行等待的,如果主线程不等待,即会引起类似于僵尸进程的问题,导致内存泄漏
//     // while (true)
//     // {
//     //     cout << "main线程: "
//     //          << " running ... " << endl;
//     //     sleep(1);
//     // }
//     return 0;
// }

/*
2023/8/16
2023/8/17
2023/8/24
lesson 36(1-14) 生产者消费者模型
基于BlockingQueue的生产者消费者模型
对lock的封装 - lockGuard
新知识 - 函数内大括号导致 提前析构 这又是一个局部
*/

#include "BlockQueue.hpp"
#include "Task.hpp"
#include <pthread.h>
#include <time.h>

int myAdd(int x, int y)
{
    return x + y;
}

void *consumer(void *args)
{
    BlockQueue<Task> *bqueue = (BlockQueue<Task> *)args;

    while (true)
    {
        // 获取任务
        Task t;
        bqueue->pop(&t);
        // 完成任务
        std::cout << pthread_self() << " consumer " << t.x_ << " + " << t.y_ << " = " << t() << std::endl;
        // sleep(1);
    }
    return nullptr;
}

void *productor(void *args)
{
    BlockQueue<Task> *bqueue = (BlockQueue<Task> *)args;
    while (true)
    {
        // 制作任务
        int x = rand() % 10 + 1; // 1~10
        usleep(rand() % 1000);   // 为了数据的随机性 计算机跑得很快
        int y = rand() % 5 + 1;

        // int x, y;
        // std::cout << "Please Enter x: ";
        // std::cin >> x;
        // std::cout << "Please Enter y: ";
        // std::cin >> y;
        Task t(x, y, myAdd);
        // 生产任务
        bqueue->push(t);
        // 输出信息
        std::cout << pthread_self() << " productor " << t.x_ << " + " << t.y_ << " = ?" << std::endl;

        sleep(1);
    }

    return nullptr;
}

int main()
{
    srand(time(nullptr) ^ getpid() ^ 0x32457); // 生成随机数种子
    BlockQueue<Task> *bqueue = new BlockQueue<Task>();
    pthread_t c[2], p[2];

    pthread_create(c, nullptr, consumer, bqueue);
    pthread_create(c + 1, nullptr, consumer, bqueue);

    pthread_create(p, nullptr, productor, bqueue);
    pthread_create(p + 1, nullptr, productor, bqueue);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);

    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);

    return 0;
}

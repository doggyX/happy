/*
2023/8/28
2023/8/29
lesson 38(2-08) 生产者消费者模型
基于环形队列的生产者消费者模型
信号量本质是一把计数器-> 计数器的意义是什么？？可以不用进入临界区，就可以得知资源情况，甚至可以减少临界区内部的判断！
以前 申请锁 -> 判断与访问(考虑条件变量) -> 释放锁 --> 本质是我们并不清楚临界资源的情况！
信号量要提前预设资源的情况，而且在pv变化过程中，我们可以在外部就能知晓临界资源的情况！(pv为原子操作)
*/

#include "ringQueue.hpp"

void *consumer(void *args)
{
    RingQueue<int> *rq = (RingQueue<int> *)args;
    while (true)
    {
        sleep(1);
        int x;
        // 1. 从环形队列中获取任务或者数据
        rq->pop(&x);
        // 2. 进行一定的处理 -- 不要忽略它的时间消耗问题
        std::cout << "消费: " << x << " [" << pthread_self() << "]" << std::endl;
    }
}

void *productor(void *args)
{
    RingQueue<int> *rq = (RingQueue<int> *)args;
    while (true)
    {
        // 1. 构建数据或者任务对象 -- 一般是可以从外部来 -- 不要忽略它的时间消耗问题
        int x = rand() % 100 + 1;
        std::cout << "生产: " << x << " [" << pthread_self() << "]" << std::endl;

        // 2. 推送到环形队列中
        rq->push(x); // 完成生产的过程
    }
}

int main()
{
    srand((uint64_t)time(nullptr) ^ getpid());
    RingQueue<int> *rq = new RingQueue<int>();
    rq->debug();
    pthread_t c[3], p[2];
    pthread_create(c, nullptr, consumer, (void *)rq);
    pthread_create(c + 1, nullptr, consumer, (void *)rq);
    pthread_create(c + 2, nullptr, consumer, (void *)rq);
    pthread_create(p, nullptr, productor, (void *)rq);
    pthread_create(p + 1, nullptr, productor, (void *)rq);

    for (int i = 0; i < 3; i++)
        pthread_join(c[i], nullptr);
    for (int i = 0; i < 2; i++)
        pthread_join(p[i], nullptr);

    return 0;
}
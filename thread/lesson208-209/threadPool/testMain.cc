/*
2023/8/29
lesson 39(2-09) 生产者消费者模型
typedef std::function<void(void *)> func_t;
函数对象 <返回值类型 (参数类型)>
typedef void *(*func_t)(void *); // 定义的是函数指针
注意 (*func_t) 又有括号 又有*

*/

#include "threadPool.hpp"
#include "Task.hpp"
#include "log.hpp"
#include <iostream>
#include <ctime>
#include <stdlib.h>

int main()
{
    // logMessage(NORMAL, "%s%d%c%f", "这是一条日志信息", 1234, 'c', 3.14);
    srand((unsigned long)(nullptr) ^ getpid());
    // std::cout << "hello thread pool" << std::endl;

    ThreadPool<Task> *tp = new ThreadPool<Task>();

    tp->run();

    while (true)
    {
        // 生产的过程, 制作任务是需要花时间的
        int x = rand() % 100 + 1;
        usleep(7721);
        int y = rand() % 30 + 1;

        Task t(x, y, [](int x, int y) -> int
               { return x + y; });
        logMessage(DEBUG, "制作任务完成 : %d + %d = ? ", x, y);
        // 推送任务到线程池中
        tp->pushTask(t);
        sleep(1);
    }

    return 0;
}
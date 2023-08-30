/*
2023/8/8
lesson 32(1-09) 线程开头
pthread_creat()
实验发现 snprintf会在末尾加上\0
*/
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <cstdio>

using namespace std;

void *threadRun(void *args)
{
    const string name = (char *)args;
    while (true)
    {
        cout << name << ", pid : " << getpid() << "\n"
             << endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid[5]; // unsigned long
    char name[64];
    for (int i = 0; i < 5; i++)
    {
        snprintf(name, sizeof name, "%s-%d", "thread", i);
        pthread_create(tid + i, nullptr, threadRun, (void *)name);
        sleep(1); // 缓解传参的bug问题
    }

    // 以前的概念是父进程和子进程, 这里的概念是主线程和新线程
    while (true)
    {
        cout << "main thread, pid: " << getpid() << endl;
        sleep(3);
    }
}
#ifndef _SEM_HPP_
#define _SEM_HPP_

#include <iostream>
#include <semaphore.h> // 也需要引入原生线程库 是原生线程库提供的

class Sem
{
public:
    Sem(int value)
    {
        sem_init(&sem_, 0, value); // 0 表示不需要共享
    }

    ~Sem()
    {
        sem_destroy(&sem_);
    }

    void p()
    {
        sem_wait(&sem_);
    }

    void v()
    {
        sem_post(&sem_);
    }

private:
    sem_t sem_;
};

#endif
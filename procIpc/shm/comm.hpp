#pragma once

#include <iostream>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include "Log.hpp"

using namespace std;

#define PATH_NAME "/home/acs" // 最好要保证自己有访问权限
#define PROJ_ID 0x66
#define SHM_SIZE 4096 // 共享内存的大小，最好是页的整数倍（4096即(4KB)）

#define FIFO_NAME "./fifo"

class Init // 创建了一个命名管道
{
public:
    Init()
    {
        umask(0);
        int n = mkfifo(FIFO_NAME, 0666);
        assert(n == 0);
        (void)n;
        Log("create fifo success ", NOTICE) << endl;
    }
    ~Init()
    {
        Log("remove fifo success ", NOTICE) << endl;
        unlink(FIFO_NAME);
    }
};

#define READ O_RDONLY
#define WRITE O_WRONLY

int OpenFIFO(std::string pathname, int flags)
{
    int fd = open(pathname.c_str(), flags);
    assert(fd >= 0);
    return fd;
}

void Wait(int fd) // 收信号 -- 那我不夯住了
{
    Log("等待中...", NOTICE) << endl;
    uint32_t temp = 0; // 32个比特位 - 4字节
    ssize_t s = read(fd, &temp, sizeof(uint32_t));
    assert(s == sizeof(uint32_t));
}

void Signal(int fd) // 发信号 -- 不用等啦
{
    uint32_t temp = 1;
    ssize_t s = write(fd, &temp, sizeof(uint32_t));
    assert(s == sizeof(uint32_t));
    (void)s;
    Log("唤醒中...", NOTICE) << endl;
}

void CloseFIFO(int fd)
{
    close(fd);
}

#include "comm.hpp"

string TransToHex(key_t k)
{
    char buffer[32];
    snprintf(buffer, sizeof buffer, "0x%x", k);
    return buffer;
}

// 是全局变量 ，对应的程序在加载的时候，会自动构建全局变量，就要调用该类的构造函数 -- 创建管道
// 程序退出时，全局变量会被析构，自动调用析构函数，会自动删除管道文件
Init init;

int main()
{
    // 1. 创建公共的Key值
    key_t k = ftok(PATH_NAME, PROJ_ID);
    assert(k != -1);
    Log("creare key done", DEBUG) << " server key : " << TransToHex(k) << endl;

    // 2. 创建共享内存 -- 建议要创建一个全新的共享内存 -- 通信的发起者
    int shmid = shmget(k, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666); // 0666是操作权限-perms
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    Log("creare shm done", DEBUG) << " shmid : " << shmid << endl;

    // sleep(10);
    // 3. 将指定的共享内存，挂接到自己的地址空间
    char *shmaddr = (char *)shmat(shmid, nullptr, 0);
    Log("attach shm done", DEBUG) << " shmid : " << shmid << endl;
    // sleep(10);

    // 这里就是通信的逻辑了
    // 将共享内存当成一个大字符串
    // 结论1： 只要是通信双方使用shm, 一方在client端写入，另一方能够直接在server端收到，速度快，减少了使用系统接口的拷贝(细说是有很多次的)
    // 导致共享内存是所有进程间通信（IPC），速度最快的（不需要过多的拷贝）
    // 结论2： 共享内存缺乏访问控制！（server一直在读--与管道通信不同） 二者甚至都不知道对方的存在。
    // 会带来并发问题

    int fd = OpenFIFO(FIFO_NAME, READ);

    while (true)
    {
        Wait(fd);
        printf("%s\n", shmaddr);
        if (strcmp(shmaddr, "quit") == 0)
            break;
        // sleep(1);
    }

    // 4. 将指定的共享内存，从自己的地址空间中，去关联
    int n = shmdt(shmaddr);
    assert(n != -1);
    Log("detach shm done", DEBUG) << " shmid : " << shmid << endl;
    // last. 删除共享内存,IPC_RMID非常强势，即使是有简称和当前的shm挂接，依旧删除共享内存
    n = shmctl(shmid, IPC_RMID, nullptr);
    assert(n != -1);
    (void)n;
    Log("delete shm done", DEBUG) << " shmid : " << shmid << endl;

    CloseFIFO(fd);
    return 0;
}
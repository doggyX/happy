#include "comm.hpp"
int main()
{
    Log("child pid is : ", DEBUG) << getpid() << endl;
    key_t k = ftok(PATH_NAME, PROJ_ID);

    if (k < 0)
    {
        Log("creare key failed", ERROR) << " client key : " << k << endl;
        exit(1);
    }
    Log("creare key done", DEBUG) << " client key : " << k << endl;

    int shmid = shmget(k, SHM_SIZE, 0); // 默认获取

    // 获取共享内存
    if (shmid < 0)
    {
        Log("creare shm done", ERROR) << " client key : " << k << endl;
        exit(2);
    }

    char *shmaddr = (char *)shmat(shmid, nullptr, 0);
    if (shmaddr == nullptr)
    {
        Log("attach shm failed", ERROR) << " client key : " << k << endl;
        exit(3);
    }
    Log("attach shm success", DEBUG) << " client key : " << k << endl;
    // sleep(10);

    int fd = OpenFIFO(FIFO_NAME, WRITE);
    // 使用
    // client将共享内存看做一个char类型的buffer
    while (true)
    {

        ssize_t s = read(0, shmaddr, SHM_SIZE - 1);
        if (s > 0)
        {
            shmaddr[s - 1] = 0;               // \n 也要置为0
            Signal(fd);                       // 唤醒
            if (strcmp(shmaddr, "quit") == 0) // 我收到quit,server也执行quit
                break;
        }
    }
    CloseFIFO(fd);

    // char a = 'a';
    // for (; a <= 'c'; a++)
    // {
    //     // 每一次都想shmaddr[共享内存的起始地址]写入
    //     snprintf(shmaddr, SHM_SIZE - 1, "hello server, 我是其他进程，我的pid : %d, inc: %c\n", getpid(), a);
    //     sleep(5);
    // }
    strcpy(shmaddr, "quit");
    // 去关联

    int n = shmdt(shmaddr);
    assert(n != -1);
    (void)n;
    Log("detach key done", DEBUG) << " client key : " << k << endl;
    // sleep(10);

    // client 就不用删除shm了， server删了

    return 0;
}
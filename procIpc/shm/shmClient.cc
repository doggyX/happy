#include "comm.hpp"

int main()
{
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
    sleep(10);

    // 使用

    // 去关联

    int n = shmdt(shmaddr);
    assert(n != -1);
    (void)n;
    Log("detach key done", DEBUG) << " client key : " << k << endl;
    sleep(10);

    // client 就不用删除shm了， server删了

    return 0;
}
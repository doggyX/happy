#include "comm.hpp"

string TransToHex(key_t k)
{
    char buffer[32];
    snprintf(buffer, sizeof buffer, "0x%x", k);
    return buffer;
}
int main()
{
    // 1. 创建公共的Key值
    key_t k = ftok(PATH_NAME, PROJ_ID);
    assert(k != -1);
    Log("creare key done", DEBUG) << " server key : " << k << endl;

    // 2. 创建共享内存 -- 建议要创建一个全新的共享内存 -- 通信的发起者
    int shmid = shmget(k, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666); // 0666是操作权限-perms
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    Log("creare shm done", DEBUG) << " shmid : " << shmid << endl;

    sleep(10);
    // 3. 将指定的共享内存，挂接到自己的地址空间
    char *shmaddr = (char *)shmat(shmid, nullptr, 0);
    Log("attach shm done", DEBUG) << " shmid : " << shmid << endl;
    sleep(10);

    // 这里就是通信的逻辑了

    // 4. 将指定的共享内存，从自己的地址空间中，去关联
    int n = shmdt(shmaddr);
    assert(n != -1);
    Log("detach shm done", DEBUG) << " shmid : " << shmid << endl;
    // last. 删除共享内存,IPC_RMID非常强势，即使是有简称和当前的shm挂接，依旧删除共享内存
    n = shmctl(shmid, IPC_RMID, nullptr);
    assert(n != -1);
    (void)n;
    Log("delete shm done", DEBUG) << " shmid : " << shmid << endl;
    return 0;
}
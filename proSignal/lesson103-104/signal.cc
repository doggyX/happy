/*
2023/7/28
lesson 29(1-04) 信号
用signal函数来修改信号的行为
区分不同类型的kill(core dump)
*/

#include <iostream>
#include <signal.h>
#include <unistd.h>   // getpid()
#include <sys/wait.h> // 进程等待

using namespace std;

void catchSig(int signum)
{
    cout << "进程捕捉到了一个信号，正在处理中： " << signum << " Pid : " << getpid() << endl;
}

int main()
{
    // // signal(2, fun); // fun是自定义回调函数
    // signal(SIGINT, catchSig); // 一般不关心返回值
    // // 意义：我把2这个信号的行为改了
    // // 注意：我在这里调用signal函数，仅仅是修改进程对特定信号的后续处理动作，不是直接调用对应的处理动作，则如果后续没有任何SIGINT信号产生，catchSig不会被调用

    // // 3 退出 ctrl + '\'
    // signal(SIGQUIT, catchSig);

    // while (true)
    // {
    //     cout << "我是一个进程，我正在运行... Pid: " << getpid() << endl;
    //     sleep(1);
    // }

    pid_t id = fork();
    cout << "father id : " << getpid() << endl;
    if (id == 0)
    {
        sleep(30);
        int a = 100;
        a /= 0;
        exit(0);
    }

    int status = 0;
    waitpid(id, &status, 0);
    cout << "父进程: " << getpid() << " 子进程: " << id << "exit sig: " << (status & 0x7f) << " is core: " << ((status >> 7) & 1) << endl;

    return 0;
}
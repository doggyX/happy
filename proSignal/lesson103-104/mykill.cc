/*
2023/7/28
lesson 29(1-04) 信号
用系统调用接口来kill进程
raise 自己给自己发信号的接口
abort 给自己发送确切的abort信号
alarm 设定闹钟 发信号 默认行为是term 通过闹钟可实现定时器功能
硬件异常产生信号 fpe nullptr(段错误-segment...)
*/
#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>   // abort()
#include <functional> // function<>
#include <vector>

using namespace std;

typedef function<void()> func; // 定义了一个function类型 返回值为void, 无参数, typedef为fun

vector<func> callbacks;

uint64_t number = 0;
void Usage(string proc)
{
    cout << "Usage:\r\n\t" << proc << " signnumber processid" << endl;
}

void showCount()
{
    // cout << "进程捕捉到了一个信号，正在处理中： " << signum << " Pid : " << getpid() << endl;
    cout << "final number : " << number << endl; // 没有IO来测试算力
}

void showLog()
{
    cout << "这个是日志功能" << endl;
}

void logUser()
{
    if (fork() == 0)
    {
        execl("/usr/bin/who", "who", nullptr);
        exit(1);
    }
}

// 定时器功能
void catchSig(int signum)
{

    for (auto &f : callbacks)
    {
        f();
    }

    alarm(1); // 重设闹钟
}

void handler(int signum)
{
    sleep(1);
    cout << "获得了一个信号" << signum << endl;
    // exit(1);
}

int main(int argc, char *argv[])
{

    // signal(SIGFPE, handler);

    // int a = 100;
    // a /= 0;

    // signal(SIGSEGV, handler);
    // int *p = nullptr; //
    // *p = 100;

    // while (true)
    //     sleep(1);

    // // 验证得只有到了一万多次
    // // 为什么这么慢? cout + 网络发送 = IO

    // signal(SIGALRM, catchSig);
    alarm(1); // 设定了一个闹钟,一旦触发，就移除了
    while (true)
        sleep(1);
    // callbacks.push_back(showCount);
    // callbacks.push_back(showLog);
    // callbacks.push_back(logUser);

    // // int number = 0;
    // while (true)
    // {
    //     number++;
    //     // cout << "number : " << number++ << endl;
    // }

    // cout << "我开始运行了" << endl;
    // sleep(1);
    // abort(); // raise(6) kill(getpid(), 6) 通常用来终止进程
    // raise(8);
    //  if (argc != 3)
    //  {
    //      Usage(argv[0]);
    //      exit(1);
    //  }

    // int signumber = atoi(argv[1]);
    // int procid = atoi(argv[2]);

    // kill(procid, signumber);
}
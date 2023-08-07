/*
2023/7/31
lesson 30(1-05) 信号保存
SIG_IGN 行为-忽略
验证尝试捕捉所有信号 9号不可捕捉
对于阻塞(屏蔽)函数的使用 认识 信号屏蔽字 和 味觉信号集 两个位图(sigset_t)
验证将所有信号阻塞 会不会导致进程无法被结束? 不会
1. 9号不会被捕捉，不可被屏蔽 SIGKILL
2. 实验发现19也不可被屏蔽 20也不可屏蔽，但似乎没有终止进程
2023/8/4 完成
*/
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <cassert>

using namespace std;
// 静态函数 在函数的返回类型前加上关键字static,函数就被定义成为静态函数。函数的定义和声明默认情况下是extern的，但静态函数只是在声明他的文件当中可见，不能被其他文件所用。

static void handler(int signum)
{
    cout << "捕捉信号 : " << signum << endl;
}

static void showPending(sigset_t &pending)
{
    for (int sig = 1; sig <= 31; sig++)
    {
        if (sigismember(&pending, sig))
            cout << "1";
        else
            cout << "0";
    }

    cout << endl;
}

static void blockSig(int sig)
{
    // 将所有信号阻塞 会不会导致进程无法被结束?
    sigset_t bset;
    sigemptyset(&bset);
    sigaddset(&bset, sig);
    int n = sigprocmask(SIG_BLOCK, &bset, nullptr);
    assert(n == 0); // release assert无用
    (void)n;
}

int main()
{
    // 3
    for (int sig = 1; sig <= 31; sig++)
    {
        blockSig(sig);
    }
    sigset_t pending;
    while (true)
    {
        sigpending(&pending);
        showPending(pending);
        sleep(1);
    }
    // 2
    // // 0. 方便测试, 捕捉2信号, 不要退出
    // signal(2, handler);
    // // 1. 定义信号集对象
    // sigset_t bset, obset; // block oldset/outputset
    // sigset_t pending;
    // // 2. 初始化
    // sigemptyset(&bset);
    // sigemptyset(&obset);
    // sigemptyset(&pending);

    // // 3. 添加要进行屏蔽的信号
    // sigaddset(&bset, 2);
    // // 4. 设置set到内核中对应的进程内部
    // int n = sigprocmask(SIG_BLOCK, &bset, &obset);
    // assert(n == 0); // release assert无用
    // (void)n;
    // cout << "block 2 号信号成功..., pid: " << getpid() << endl;
    // // 5. 重复打印当前进程的pending信号集
    // int count = 0;
    // while (true)
    // {
    //     // 5.1 获取当前进程的pending信号集
    //     sigpending(&pending);
    //     // 5.2 显示pending信号集中的没有被递达的信号
    //     showPending(pending);
    //     sleep(1);
    //     count++;

    //     if (count == 20)
    //     {
    //         // 默认情况下, 回复对于2号信号的block的时候，确实会进行递达
    //         // 但是2号信号是终止 所以方便测试, 我们捕捉2号信号, 不要终止
    //         cout << "解除对于2号信号的block" << endl;
    //         int n = sigprocmask(SIG_SETMASK, &obset, nullptr);
    //         assert(n == 0); // release assert无用
    //         (void)n;
    //     }
    // }

    return 0;
}
// 1
// void catchSig(int signum)
// {
//     cout << "获得了一个信号: " << signum << endl;

//     //     switch(signum)
//     //     {
//     //         case SIGINT:
//     //             break;
//     //     }
// }

// int main()
// {
//     for (int i = 1; i <= 31; i++)
//         signal(i, catchSig); // 把信号全部捕捉

//     while (true)
//         sleep(1);

//     // // signal(2, hander);
//     // signal(2, SIG_IGN); // 忽略 (sighander_t) 1

//     return 0;
// }
/*
2023/8/7
lesson 31(1-08) 信号操作
sigaction函数和结构体
结论和实验证明: 信号在被处理的这段时间 他自己也被屏蔽了
用sigaction的mask还可以将其他信号暂时屏蔽
volitile 不让编译器自作主张优化
O优化 是在编译阶段就优化好了,不是执行的时候
*/
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void handler(int signum)
{
    cout << "子进程退出: " << signum << endl;
}

int main()
{
    signal(SIGCHLD, handler);

    if (fork() == 0)
    {
        sleep(1);
        exit(0);
    }

    while (true)
        sleep(1);
}

// volatile int flag = 0; // 作用 : 有可能被编译器优化(开O1就优化了), 这里不让编译器自作主张优化
// // O优化 是在编译阶段就优化好了,不是执行的时候

// void changeFlag(int signum)
// {
//     (void)signum;
//     cout << "change flag: " << flag;
//     flag = 1;
//     cout << "->" << flag << endl;
// }

// int main()
// {
//     signal(2, changeFlag);
//     while (!flag)
//         ;
//     cout << "进程正常退出后: " << flag << endl;
// }

// void showPending(sigset_t *pending)
// {
//     for (int sig = 1; sig <= 31; sig++)
//     {
//         if (sigismember(pending, sig))
//             cout << "1";
//         else
//             cout << "0";
//     }
//     cout << endl;
// }

// void handler(int signum)
// {
//     cout << "获取了一个信号: " << signum << endl;
//     cout << "获取了一个信号: " << signum << endl;
//     cout << "获取了一个信号: " << signum << endl;
//     cout << "获取了一个信号: " << signum << endl;
//     cout << "获取了一个信号: " << signum << endl;
//     cout << "获取了一个信号: " << signum << endl;

//     sigset_t pending;

//     int c = 20;
//     while (true)
//     {
//         sigpending(&pending);
//         showPending(&pending);
//         c--;
//         if (!c)
//             break;
//         sleep(1);
//     }
// }

// int main()
// {
//     signal(2, SIG_IGN);
//     cout << "pid : " << getpid() << endl;
//     // c++是可以省略struct的
//     struct sigaction act, oact;
//     // 初始化 内核数据结构 用户栈定义的
//     act.sa_flags = 0;
//     sigemptyset(&act.sa_mask);
//     act.sa_handler = handler;

//     sigaddset(&act.sa_mask, 3);
//     sigaddset(&act.sa_mask, 4);
//     sigaddset(&act.sa_mask, 5);
//     sigaddset(&act.sa_mask, 6);
//     sigaddset(&act.sa_mask, 7);

//     // 设置进当前调用进程的pcb中
//     sigaction(2, &act, &oact);

//     cout << "default action : " << (int)(oact.sa_handler) << endl; // 本来默认就是0 oldact

//     while (true)
//         sleep(1);

//     return 0;
// }
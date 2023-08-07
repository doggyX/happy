#include <iostream>
#include <signal.h>

using namespace std;

// 15
int main()
{
    cout << "pid : " << getpid() << endl;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, 2);
    sigaddset(&set, 40);

    sigprocmask(SIG_BLOCK, &set, nullptr);

    while (true)
        sleep(1);
    return 0;
}

// 14
// void sigcb(int signum)
// {
//     cout << "接收到一个信号 : " << signum << endl;
// }

// int main()
// {
//     struct sigaction act, oact;

//     act.sa_flags = 0;
//     sigemptyset(&act.sa_mask);
//     sigaddset(&act.sa_mask, SIGINT);
//     act.sa_handler = sigcb;

//     sigaction(SIGINT, &act, &oact);

//     while (true)
//         sleep(1);
//     return 0;
// }

// 13
// void sigcb(int signum)
// {
//     cout << "接收到一个信号 : " << signum << endl;
// }

// int main()
// {
//     signal(SIGINT, sigcb);
//     while (true)
//         sleep(1);
//     return 0;
// }
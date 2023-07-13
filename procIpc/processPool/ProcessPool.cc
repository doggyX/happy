#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include "Task.hpp"

#define PROCESS_NUM 5

using namespace std;

int waitCommand(int waitFd, bool &quit) // 如果对方不发 我们就阻塞
{
    uint32_t command = 0;
    ssize_t s = read(waitFd, &command, sizeof command); // woc 居然是写进command要& 平时写进char*都是直接写所以没感觉

    if (s == 0)
    {
        quit = true;
        return -1;
    }
    assert(s == sizeof command);
    return command;
}

void sendAndWakeup(pid_t who, int fd, uint32_t command)
{
    write(fd, &command, sizeof command);
    cout << "main process call process " << who << " excute " << desc[command] << " through " << fd << endl;
}

int main()
{
    // 代码中关于fd的使用 有一个小问题 但不影响使用 可以找一下
    load();
    // pid pipfd键值对
    vector<pair<pid_t, int>> slots;

    // 创建多个进程
    for (int i = 0; i < PROCESS_NUM; i++)
    {
        // 创建管道
        int pipfd[2] = {0};
        int n = pipe(pipfd);
        assert(n == 0);
        (void)n;

        pid_t id = fork();
        assert(id != -1);

        if (id == 0)
        {
            // 关闭写通道
            close(pipfd[1]);

            // 子进程

            while (true)
            {
                bool quit = false;
                int command = waitCommand(pipfd[0], quit); // 如果对方不发 我们就阻塞

                // quit
                if (quit == true)
                {
                    break;
                }
                // 执行对应命令
                if (command >= 0 && command < handlerSize())
                {
                    callbacks[command]();
                }
                else
                {
                    cout << "非法command: " << command << endl;
                }
            }
            exit(0);
        }

        close(pipfd[0]); // 可以使用pipfd[1]
        slots.push_back(pair<pid_t, int>(id, pipfd[1]));
    }

    // 父进程派发任务

    srand((unsigned long)time(nullptr) ^ getpid() ^ 23323123123L); // 为了让随机数更加随机

    while (true)
    {

        // int select;
        int command;

        // 选择任务 (可以从网络来)
        command = rand() % handlerSize();
        // 选择进程 (随机分配)
        int choice = rand() % slots.size();
        // 把任务给指定的进程
        sendAndWakeup(slots[choice].first, slots[choice].second, command);

        sleep(1);

        // cout << "###################################" << endl;
        // cout << "# 1.show functions 2.send command #" << endl;
        // cout << "###################################" << endl;
        // cout << "Please Select> ";
        // cin >> select;
        // if (select == 1)
        //     showHandlers();
        // else if (select == 2)
        // {
        //     cout << "Enter Your Command> ";
        //     // 选择任务
        //     cin >> command;
        //     // 选择进程
        //     int choice = rand() % slots.size();
        //     // 把任务给指定的进程
        //     sendAndWakeup(slots[choice].first, slots[choice].second, command);
        // }
        // else
        // {
        // }
    }

    // 关闭fd 结束所有子进程
    for (const auto &slot : slots)
    {
        close(slot.second);
    }

    // 回收所有的子进程信息
    for (const auto &slot : slots)
    {
        waitpid(slot.first, nullptr, 0);
    }
}